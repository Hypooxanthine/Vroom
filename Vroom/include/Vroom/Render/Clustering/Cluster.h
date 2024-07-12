#pragma once

#include <array>
#include <glm/glm.hpp>

namespace vrm
{

/**
 * @brief Represents a cluster in the clustering algorithm.
 * For each fragment, it will be required to convert its position to NDC space to check if it is inside a cluster.
 * 
 */
class Cluster
{
public:
    struct IntersectionResult
    {
        bool intersects;
        glm::vec3 nearestPointInAABB;
    };

public:
    Cluster() = default;
    Cluster(const Cluster&) = default;
    Cluster(Cluster&&) = default;
    ~Cluster() = default;

    Cluster& operator=(const Cluster&) = default;
    Cluster& operator=(Cluster&&) = default;
    
    Cluster(const glm::vec3& minAABB_VS, const glm::vec3& maxAABB_VS);

    /**
     * @brief Sets the corners of the cluster in view space.
     * 
     * @param minAABB_VS The minimum corner of the cluster in view space.
     * @param maxAABB_VS The maximum corner of the cluster in view space.
     */
    void setCorners(const glm::vec3& minAABB_VS, const glm::vec3& maxAABB_VS);

    /**
     * @brief Gets the nearest point in the AABB to a given point.
     * 
     * @param point The point to find the nearest point in the AABB in view space.
     * @return glm::vec3 The nearest point in the AABB to the given point.
     */
    glm::vec3 getNearestPointInAABB(const glm::vec3& point) const;

    /**
     * @brief Checks if a sphere in view space intersects with the cluster.
     * 
     * @param sphereCenter_VS The center of the sphere in view space.
     * @param sphereRadius The radius of the sphere (view space or world space).
     * @return IntersectionResult The result of the intersection test.
     */
    IntersectionResult intersectsSphereVS(const glm::vec3& sphereCenter_VS, float sphereRadius) const;

private:
    

private:
    glm::vec3 m_MinAABB_VS, m_MaxAABB_VS;
};

} // namespace vrm
