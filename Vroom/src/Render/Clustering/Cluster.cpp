#include "Vroom/Render/Clustering/Cluster.h"

#include <glm/gtx/norm.hpp>
#include <unordered_set>

#include "Vroom/Core/Assert.h"

namespace vrm
{

std::array<std::tuple<size_t, size_t, size_t>, 8> Cluster::s_Neighbours = {
    std::make_tuple(1, 3, 4), // 0: NearBottomLeft
    std::make_tuple(0, 2, 5), // 1: NearTopLeft
    std::make_tuple(1, 3, 6), // 2: NearTopRight
    std::make_tuple(0, 2, 7), // 3: NearBottomRight
    std::make_tuple(0, 5, 7), // 4: FarBottomLeft
    std::make_tuple(1, 4, 6), // 5: FarTopLeft
    std::make_tuple(2, 5, 7), // 6: FarTopRight
    std::make_tuple(3, 4, 6)  // 7: FarBottomRight
};

std::array<std::tuple<size_t, size_t, size_t>, 6> Cluster::s_FacesPointingInside = {
    std::make_tuple(0, 1, 2), // Near
    std::make_tuple(4, 7, 6), // Far
    std::make_tuple(0, 3, 7), // Bottom
    std::make_tuple(1, 5, 6), // Top
    std::make_tuple(0, 4, 5), // Left
    std::make_tuple(2, 6, 7)  // Right
};

Cluster::Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& invViewProjectionMatrix, float nearVal, float farVal)
{
    setCorners(nearBottomLeft, farTopRight, invViewProjectionMatrix, nearVal, farVal);
}

const glm::vec3& Cluster::getNearBottomLeft() const
{
    return m_NearBottomLeft;
}

const glm::vec3& Cluster::getFarTopRight() const
{
    return m_FarTopRight;
}

void Cluster::setCorners(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& invViewProjectionMatrix, float nearVal, float farVal)
{
    m_NearBottomLeft = nearBottomLeft;
    m_FarTopRight = farTopRight;

    // Un-linearize the depth values.
    const float invNear = 1.f / nearVal;
    const float invFar = 1.f / farVal;

    float z = (m_NearBottomLeft.z * 0.5f + 0.5f) * farVal;
    float d = (1.f / z - invNear) / (invFar - invNear);
    m_NearBottomLeft.z = d * 2.f - 1.f;

    z = (m_FarTopRight.z * 0.5f + 0.5f) * farVal;
    d = (1.f / z - invNear) / (invFar - invNear);
    m_FarTopRight.z = d * 2.f - 1.f;

    updateWSCorners(invViewProjectionMatrix);
}

bool Cluster::containsNDC(const glm::vec3& point) const
{
    return point.x >= m_NearBottomLeft.x && point.x <= m_FarTopRight.x &&
           point.y >= m_NearBottomLeft.y && point.y <= m_FarTopRight.y &&
           point.z >= m_NearBottomLeft.z && point.z <= m_FarTopRight.z;
}

glm::vec3 Cluster::getClosestPointNDC(const glm::vec3& P) const
{
    // If the point is already inside the cluster, we return it: it is the closest point.
    if (containsNDC(P))
        return P;
    
    // We search for (x, y, z), the point in the cluster that is closest to P.
    // In NDC, the cluster is a box aligned with the axes.
    // So we just clamp the coordinates of P to the cluster.
    return glm::vec3(glm::clamp(P.x, m_NearBottomLeft.x, m_FarTopRight.x),
                     glm::clamp(P.y, m_NearBottomLeft.y, m_FarTopRight.y),
                     glm::clamp(P.z, m_NearBottomLeft.z, m_FarTopRight.z));
}

bool Cluster::insersectsSphereNDC(const glm::vec3& center, float radius) const
{
    // Getting the closest point in the cluster to the sphere center.
    auto C = getClosestPointNDC(center);

    // Now we calculate the squared distance between the sphere center and C.
    float sqrDistance = (C.x - center.x) * (C.x - center.x) +
                        (C.y - center.y) * (C.y - center.y) +
                        (C.z - center.z) * (C.z - center.z);

    // If the sphere center is outside the cluster, the radius has to be big enough to intersect the cluster.
    // If the sphere center is inside the cluster, then the distance is 0. This means it will return true for any positive radius.
    return sqrDistance < radius * radius;
}

bool Cluster::containsWS(const glm::vec3& P) const
{
    // We check if P is inside all the faces of the cluster that are pointing inside.
    for (const auto& [iA, iB, iC] : s_FacesPointingInside)
    {
        const glm::vec3& AB = m_WSCorners[iB] - m_WSCorners[iA];
        const glm::vec3& AC = m_WSCorners[iC] - m_WSCorners[iA];
        const glm::vec3& AP = P               - m_WSCorners[iA];
        if (glm::determinant(glm::mat3(AB, AC, AP)) < 0.f)
            return false;
    }

    return true;
}

glm::vec3 Cluster::getClosestPointWS(const glm::vec3& P) const
{
    std::unordered_set<size_t> restrictedPoints;
    for (const auto& [iA, iB, iC] : s_FacesPointingInside)
    {
        const glm::vec3 AB = m_WSCorners[iB] - m_WSCorners[iA];
        const glm::vec3 AC = m_WSCorners[iC] - m_WSCorners[iA];
        const glm::vec3 AP = P               - m_WSCorners[iA];
        if (glm::determinant(glm::mat3(AB, AC, AP)) < 0.f) // P is outside the face.
        {
            restrictedPoints.insert(iA);
            restrictedPoints.insert(iB);
            restrictedPoints.insert(iC);
        }
    }

    // If P is inside the cluster, we return it (it is the closest point to itself in the cluster).
    // <=> restrictedPoints is empty.
    if (restrictedPoints.empty())
        return P;

    // We filled restrictedPoints with the indices of the corners of the faces separating P from the inside of the cluster.
    // Let's recall that P can't be inside the cluster at this point.
    // If P is "inside" a face F, either P is inside the cluster, or there is at least one face between P and F. 
    // Since P is not inside the cluster, and the cluster is convex, there is a single face F' between P and F, and P is "outside" F'.
    // Since faces shouldn't intersect, the closest point from P to F and F' has to be on F'. So we can restrict the search to the corners of F'.
    
    // Finally, for Fi in s_FacesPointingInside, restrictedPoints contains the corners of Fi (no repeats).
    
    // Searching for D, the cluster corner that is closest to P.
    // iD is the index of D.
    size_t iD = 0;
    float minDistance = glm::length2(m_WSCorners[iD] - P);

    for (const size_t& i : restrictedPoints)
    {
        float distance = glm::length2(m_WSCorners[i] - P);
        if (distance < minDistance)
        {
            minDistance = distance;
            iD = i;
        }
    }
    
    const glm::vec3& D = m_WSCorners[iD];

    // Searching for A, B, C, the three neighbours of D.
    auto [iA, iB, iC] = s_Neighbours[iD];
    const glm::vec3& A = m_WSCorners[iA];
    const glm::vec3& B = m_WSCorners[iB];
    const glm::vec3& C = m_WSCorners[iC];

    const glm::vec3 DA = A - D;
    const glm::vec3 DB = B - D;
    const glm::vec3 DC = C - D;

    // We will sort a collection by comparing cosines, which will be done by comparing dot products.
    // If we do not normalize DP only, there is no problem because we will compare dot products all multiplied by
    // ||DP||, which is a constant positive value. If we have cosines x < y z, then we have x * ||DP|| < y * ||DP|| < z * ||DP|| too.
    // The positivity of the cosine is preserved too when multiplying by a positive constant.
    const glm::vec3 DP = P - D;

    // We have a collection of sets { Neighbour, normalized Direction, Cosine multiplied by ||DP|| }.
    std::array<std::tuple<glm::vec3, glm::vec3, float>, 3> candidates = {
        std::make_tuple(A, glm::normalize(DA), glm::dot(DP, DA)),
        std::make_tuple(B, glm::normalize(DB), glm::dot(DP, DB)),
        std::make_tuple(C, glm::normalize(DC), glm::dot(DP, DC))
    };

    // candidates[0] has the biggest cosine, candidates[2] has the smallest cosine.
    std::sort(s_Neighbours.begin(), s_Neighbours.end(), [](const auto& a, const auto& b)
    {
        return std::get<2>(a) > std::get<2>(b);
    });
    
    // We will add to D the orthogonal projection of P on on each direction associated with a positive cosine,
    // starting with the biggest cosine, and a maximum of two directions.
    // The final value will be D if there is 0 positive cosine, on an edge if there is 1 positive cosine, and on a face if there are 2 positive cosines.
    glm::vec3 out = D;

    uint8_t count = 0;
    for (const auto& [neighbour, ndirection, cosineTimesDPLength] : candidates)
    {
        if (cosineTimesDPLength > 0.f)
        {
            out += glm::dot(DP, ndirection) * ndirection;
            ++count;
        }
        else
            // The cosine is negative, so the next ones will be too.
            return out;

        if (count == 2)
            return out;
    }

    return out;
}

bool Cluster::intersectsSphereWS(const glm::vec3& center, float radius) const
{
    // Getting the closest point in the cluster to the sphere center.
    auto C = getClosestPointWS(center);
    if (C == center)
        return true;

    // Now we calculate the squared distance between the sphere center and C.
    float sqrDistance = (C.x - center.x) * (C.x - center.x) +
                        (C.y - center.y) * (C.y - center.y) +
                        (C.z - center.z) * (C.z - center.z);

    // If the sphere center is outside the cluster, the radius has to be big enough to intersect the cluster.
    // If the sphere center is inside the cluster, then the distance is 0. This means it will return true for any positive radius.
    return sqrDistance < radius * radius;
}

void Cluster::updateWSCorners(const glm::mat4& invViewProjectionMatrix)
{
    // NearBottomLeft
    m_WSCorners[0] = glm::vec3(m_NearBottomLeft.x, m_NearBottomLeft.y, m_NearBottomLeft.z);
    // NearTopLeft
    m_WSCorners[1] = glm::vec3(m_NearBottomLeft.x, m_FarTopRight.y, m_NearBottomLeft.z);
    // NearTopRight
    m_WSCorners[2] = glm::vec3(m_FarTopRight.x, m_FarTopRight.y, m_NearBottomLeft.z);
    // NearBottomRight
    m_WSCorners[3] = glm::vec3(m_FarTopRight.x, m_NearBottomLeft.y, m_NearBottomLeft.z);
    // FarBottomLeft
    m_WSCorners[4] = glm::vec3(m_NearBottomLeft.x, m_NearBottomLeft.y, m_FarTopRight.z);
    // FarTopLeft
    m_WSCorners[5] = glm::vec3(m_NearBottomLeft.x, m_FarTopRight.y, m_FarTopRight.z);
    // FarTopRight
    m_WSCorners[6] = glm::vec3(m_FarTopRight.x, m_FarTopRight.y, m_FarTopRight.z);
    // FarBottomRight
    m_WSCorners[7] = glm::vec3(m_FarTopRight.x, m_NearBottomLeft.y, m_FarTopRight.z);

    for (auto& corner : m_WSCorners)
    {
        glm::vec4 corner4 = invViewProjectionMatrix * glm::vec4(corner, 1.f);
        corner = glm::vec3(corner4) / corner4.w;
    }
}

} // namespace vrm