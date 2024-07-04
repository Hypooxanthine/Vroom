#include "Vroom/Render/Clustering/Cluster.h"

namespace vrm
{
    
Cluster::Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight)
    : m_NearBottomLeft(nearBottomLeft), m_FarTopRight(farTopRight)
{
}

const glm::vec3& Cluster::getNearBottomLeft() const
{
    return m_NearBottomLeft;
}

const glm::vec3& Cluster::getFarTopRight() const
{
    return m_FarTopRight;
}

void Cluster::setNearBottomLeft(const glm::vec3& nearBottomLeft)
{
    m_NearBottomLeft = nearBottomLeft;
}

void Cluster::setFarTopRight(const glm::vec3& farTopRight)
{
    m_FarTopRight = farTopRight;
}

bool Cluster::contains(const glm::vec3& point) const
{
    return point.x >= m_NearBottomLeft.x && point.x <= m_FarTopRight.x &&
           point.y >= m_NearBottomLeft.y && point.y <= m_FarTopRight.y &&
           point.z >= m_NearBottomLeft.z && point.z <= m_FarTopRight.z;
}

bool Cluster::insersectsSphere(const glm::vec3& center, float radius) const
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

} // namespace vrm