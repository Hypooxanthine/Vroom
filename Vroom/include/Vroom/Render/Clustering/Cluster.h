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
    
    Cluster(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& invViewProjectionMatrix, float nearVal, float farVal);

    const glm::vec3& getNearBottomLeft() const;
    const glm::vec3& getFarTopRight() const;

    void setCorners(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::mat4& invViewProjectionMatrix, float nearVal, float farVal);

    /**
     * @brief Checks if a point is inside the cluster where the coordinates are in NDC space.
     * 
     * @param point Point to check.
     * @return true If the point is inside the cluster.
     * @return false If the point is outside the cluster.
     */
    bool containsNDC(const glm::vec3& point) const;

    /**
     * @brief Returns the closest point in the cluster to a point where the coordinates are in NDC space.
     * 
     * @param point Point outside the cluster.
     * @return glm::vec3 Closest point in the cluster to the input point.
     */
    glm::vec3 getClosestPointNDC(const glm::vec3& point) const;

    /**
     * @brief Checks if a sphere intersects the cluster where the coordinates are in NDC space. This will also return true if the sphere is fully inside the cluster.
     * 
     * @param center Center of the sphere.
     * @param radius Radius of the sphere.
     * @return true If the sphere intersects the cluster.
     * @return false If the sphere does not intersect the cluster.
     */
    bool insersectsSphereNDC(const glm::vec3& center, float radius) const;

    /**
     * @brief Checks if a point is inside the cluster where the coordinates are in world space.
     * 
     * @param point Point to check.
     * @return true If the point is inside the cluster.
     * @return false If the point is outside the cluster.
     */
    bool containsWS(const glm::vec3& point) const;

    /**
     * @brief Returns the closest point in the cluster to a point where the coordinates are in world space.
     * @see @ref clustered_rendering for more information about the algorithm.
     * 
     * @param point Point outside the cluster.
     * @return glm::vec3 Closest point in the cluster to the input point.
     */
    glm::vec3 getClosestPointWS(const glm::vec3& point) const;

    /**
     * @brief Checks if a sphere intersects the cluster where the coordinates are in world space. This will also return true if the sphere is fully inside the cluster.
     * @see @ref clustered_rendering for more information about the algorithm.
     * 
     * @param center Center of the sphere.
     * @param radius Radius of the sphere.
     * @return true If the sphere intersects the cluster.
     * @return false If the sphere does not intersect the cluster.
     */
    bool intersectsSphereWS(const glm::vec3& center, float radius) const;

private:
    void updateWSCorners(const glm::mat4& invViewProjectionMatrix);
    

private:
    glm::vec3 m_NearBottomLeft, m_FarTopRight; // In NDC space
    std::array<glm::vec3, 8> m_WSCorners; // NearBottomLeft, NearTopLeft, NearTopRight, NearBottomRight, FarBottomLeft, FarTopLeft, FarTopRight, FarBottomRight
    static std::array<std::tuple<size_t, size_t, size_t>, 8> s_Neighbours;
    static std::array<std::tuple<size_t, size_t, size_t>, 6> s_FacesPointingInside;
};

} // namespace vrm
