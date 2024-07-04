#pragma once

#include <glm/glm.hpp>

namespace vrm
{

/**
 * @brief Represents a cluster in the clustering algorithm. Coordinates are in the NDC space.
 * For each fragment, it will be required to convert its position to NDC space to check if it is inside a cluster.
 * 
 */
class Cluster
{
public:
    Cluster() = default;
    Cluster(const Cluster&) = default;
    Cluster(Cluster&&) = default;
    ~Cluster() = default;

    Cluster& operator=(const Cluster&) = default;
    Cluster& operator=(Cluster&&) = default;
    
    Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight);

    const glm::vec3& getNearBottomLeft() const;
    const glm::vec3& getFarTopRight() const;

    void setNearBottomLeft(const glm::vec3& nearBottomLeft);
    void setFarTopRight(const glm::vec3& farTopRight);

    /**
     * @brief Checks if a point is inside the cluster.
     * 
     * @param point Point to check.
     * @return true If the point is inside the cluster.
     * @return false If the point is outside the cluster.
     */
    bool contains(const glm::vec3& point) const;

    /**
     * @brief Checks if a sphere intersects the cluster. This will also return true if the sphere is fully inside the cluster.
     * 
     * @param center Center of the sphere.
     * @param radius Radius of the sphere.
     * @return true If the sphere intersects the cluster.
     * @return false If the sphere does not intersect the cluster.
     */
    bool insersectsSphere(const glm::vec3& center, float radius) const;

private:
    glm::vec3 m_NearBottomLeft;
    glm::vec3 m_FarTopRight;

};

} // namespace vrm
