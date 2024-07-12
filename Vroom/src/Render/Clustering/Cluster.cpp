#include "Vroom/Render/Clustering/Cluster.h"

#include <glm/gtx/norm.hpp>
#include <unordered_set>

#include "Vroom/Core/Assert.h"

namespace vrm
{

Cluster::Cluster(const glm::vec3& minAABB_VS, const glm::vec3& maxAABB_VS)
{
    setCorners(minAABB_VS, maxAABB_VS);
}

void Cluster::setCorners(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight)
{
    m_MinAABB_VS = nearBottomLeft;
    m_MaxAABB_VS = farTopRight;
}

glm::vec3 Cluster::getNearestPointInAABB(const glm::vec3& point) const
{
    return glm::clamp(point, m_MinAABB_VS, m_MaxAABB_VS);
}

Cluster::IntersectionResult Cluster::intersectsSphereVS(const glm::vec3& sphereCenter_VS, float sphereRadius) const
{
    glm::vec3 closestPointInAABB = getNearestPointInAABB(sphereCenter_VS);
    float distanceSquared = glm::distance2(sphereCenter_VS, closestPointInAABB);
    return { distanceSquared < sphereRadius * sphereRadius, closestPointInAABB };
}

} // namespace vrm