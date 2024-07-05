#include "Vroom/Render/Clustering/Cluster.h"

#include "Vroom/Core/Assert.h"

namespace vrm
{
    
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

bool Cluster::containsNDC(const glm::vec3& point) const
{
    return point.x >= m_NearBottomLeft.x && point.x <= m_FarTopRight.x &&
           point.y >= m_NearBottomLeft.y && point.y <= m_FarTopRight.y &&
           point.z >= m_NearBottomLeft.z && point.z <= m_FarTopRight.z;
}

bool Cluster::insersectsSphereNDC(const glm::vec3& center, float radius) const
{
    // We search for (x, y, z), the point in the cluster that is closest to the center of the sphere.
    // x is the closest point to the center of the sphere in the x axis, and so on.
    // If the sphere center is inside the cluster, then (x, y, z) is the center of the sphere.
    float x = glm::clamp(center.x, m_NearBottomLeft.x, m_FarTopRight.x);
    float y = glm::clamp(center.y, m_NearBottomLeft.y, m_FarTopRight.y);
    float z = glm::clamp(center.z, m_NearBottomLeft.z, m_FarTopRight.z);

    // Now we calculate the squared distance between the sphere center and (x, y, z).
    // If the sphere center is inside the cluster, the distance will be 0.
    float sqrDistance = (x - center.x) * (x - center.x) +
                        (y - center.y) * (y - center.y) +
                        (z - center.z) * (z - center.z);

    // If the sphere center is outside the cluster, the radius has to be big enough to intersect the cluster.
    // If the sphere center is inside the cluster, then the distance is 0. This means it will return true for any positive radius.
    return sqrDistance < radius * radius;
}

bool Cluster::containsWS(const glm::vec3& point) const
{
    return point.x >= m_WSCorners[0].x && point.x <= m_WSCorners[6].x &&
           point.y >= m_WSCorners[0].y && point.y <= m_WSCorners[6].y &&
           point.z >= m_WSCorners[0].z && point.z <= m_WSCorners[6].z;
}

bool Cluster::intersectsSphereWS(const glm::vec3& center, float radius) const
{
    VRM_ASSERT_MSG(false, "Not implemented yet.");
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