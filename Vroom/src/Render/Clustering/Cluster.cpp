#include "Vroom/Render/Clustering/Cluster.h"

#include <glm/gtx/norm.hpp>

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

Cluster::Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& viewProjectionMatrix)
    : m_NearBottomLeft(nearBottomLeft), m_FarTopRight(farTopRight)
{
    updateWSCorners(viewProjectionMatrix);
}

const glm::vec3& Cluster::getNearBottomLeft() const
{
    return m_NearBottomLeft;
}

const glm::vec3& Cluster::getFarTopRight() const
{
    return m_FarTopRight;
}

void Cluster::setNearBottomLeft(const glm::vec3& nearBottomLeft, const glm::mat4& viewProjectionMatrix)
{
    m_NearBottomLeft = nearBottomLeft;
    updateWSCorners(viewProjectionMatrix);

}

void Cluster::setFarTopRight(const glm::vec3& farTopRight, const glm::mat4& viewProjectionMatrix)
{
    m_FarTopRight = farTopRight;
    updateWSCorners(viewProjectionMatrix);
}

void Cluster::setCorners(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& viewProjectionMatrix)
{
    m_NearBottomLeft = nearBottomLeft;
    m_FarTopRight = farTopRight;
    updateWSCorners(viewProjectionMatrix);
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

bool Cluster::containsWS(const glm::vec3& point) const
{
    VRM_ASSERT_MSG(false, "Not implemented yet.");
}

glm::vec3 Cluster::getClosestPointWS(const glm::vec3& P) const
{
    if (containsWS(P))
        return P;
    
    // Searching for D, the point in the cluster that is closest to P.
    // iD is the index of D.
    size_t iD = 0;
    float minDistance = glm::length2(m_WSCorners[iD] - P);

    for (size_t i = 1; i < m_WSCorners.size(); ++i)
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

    // If DP.DG < 0 for G in {A, B, C}, then D is the closest point.
    const glm::vec3 DP = P - D;
    const glm::vec3 DA = A - D;
    const glm::vec3 DB = B - D;
    const glm::vec3 DC = C - D;

    const float DPDA = glm::dot(DP, DA);
    const float DPDB = glm::dot(DP, DB);
    const float DPDC = glm::dot(DP, DC);

    if (DPDA < 0.f && DPDB < 0.f && DPDC < 0.f)
        return D;
    
    // Finding M, M' in {A, B, C} such that DP.DM / (||DP|| * ||DM||) and DP.DM' / (||DP|| * ||DM'||) are maximized.
    /// @todo I think we can optimize this with squared cosines.
    const float cosDPDA = DPDA / (glm::length(DP) * glm::length(DA));
    const float cosDPDB = DPDB / (glm::length(DP) * glm::length(DB));
    const float cosDPDC = DPDC / (glm::length(DP) * glm::length(DC));

    glm::vec3 DM, DMp;

    if (cosDPDA < cosDPDB && cosDPDA < cosDPDC)
    {
        DM = DB;
        DMp = DC;
    }
    else if (cosDPDB < cosDPDA && cosDPDB < cosDPDC)
    {
        DM = DA;
        DMp = DC;
    }
    else
    {
        DM = DA;
        DMp = DB;
    }

    const glm::vec3 nDM = glm::normalize(DM);
    const glm::vec3 nDMp = glm::normalize(DMp);

    // PI1 is the orthogonal projection of DP on DM.
    // PI2 is the orthogonal projection of DP on DM'.
    glm::vec3 PI1 = glm::dot(DP, nDM) * nDM;
    glm::vec3 PI2 = glm::dot(DP, nDMp) * nDMp;

    // PI0 is the orthogonal projection of P on the plane defined by D, M, M'.
    glm::vec3 PI0 = D + PI1 + PI2;

    return PI0;
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

void Cluster::updateWSCorners(const glm::mat4& viewProjectionMatrix)
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
        glm::vec4 corner4 = viewProjectionMatrix * glm::vec4(corner, 1.f);
        corner = glm::vec3(corner4) / corner4.w;
    }
}

} // namespace vrm