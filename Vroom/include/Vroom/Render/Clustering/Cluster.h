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
    Cluster() = default;
    Cluster(const Cluster&) = default;
    Cluster(Cluster&&) = default;
    ~Cluster() = default;

    Cluster& operator=(const Cluster&) = default;
    Cluster& operator=(Cluster&&) = default;
    
    Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& viewProjectionMatrix);

    const glm::vec3& getNearBottomLeft() const;
    const glm::vec3& getFarTopRight() const;

    void setNearBottomLeft(const glm::vec3& nearBottomLeft, const glm::mat4& viewProjectionMatrix);
    void setFarTopRight(const glm::vec3& farTopRight, const glm::mat4& viewProjectionMatrix);

    /**
     * @brief Checks if a point is inside the cluster where the coordinates are in NDC space.
     * 
     * @param point Point to check.
     * @return true If the point is inside the cluster.
     * @return false If the point is outside the cluster.
     */
    bool containsNDC(const glm::vec3& point) const;

    /**
     * @brief Checks if a sphere intersects the cluster where the coordinates are in NDC space. This will also return true if the sphere is fully inside the cluster.
     * 
     * @param center Center of the sphere.
     * @param radius Radius of the sphere.
     * @return true If the sphere intersects the cluster.
     * @return false If the sphere does not intersect the cluster.
     */
    bool insersectsSphereNDC(const glm::vec3& center, float radius) const;

    bool containsWS(const glm::vec3& point) const;

    bool intersectsSphereWS(const glm::vec3& center, float radius) const;

private:
    void updateWSCorners(const glm::mat4& viewProjectionMatrix);

private:
    glm::vec3 m_NearBottomLeft, m_FarTopRight; // In NDC space
    std::array<glm::vec3, 8> m_WSCorners; // NearBottomLeft, NearTopLeft, NearTopRight, NearBottomRight, FarBottomLeft, FarTopLeft, FarTopRight, FarBottomRight

};

} // namespace vrm
