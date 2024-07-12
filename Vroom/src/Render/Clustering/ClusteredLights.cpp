#include "Vroom/Render/Clustering/ClusteredLights.h"

#include <glm/gtx/string_cast.hpp>

namespace vrm
{

void ClusteredLights::setBindingPoints(int clusterInfoBindingPoint, int lightIndicesBindingPoint)
{
    m_SSBOClusterInfoSSBO.setBindingPoint(clusterInfoBindingPoint);
    m_SSBOLightIndicesSSBO.setBindingPoint(lightIndicesBindingPoint);
}

glm::vec3 intersectionLineAndZPerpendicularPlane(const glm::vec3& linePoint, const glm::vec3& lineDirection, float depth)
{
    glm::vec3 planeNormal = { 0.f, 0.f, -1.f };
    return linePoint + ( (depth - dot(planeNormal, linePoint)) / dot(planeNormal, lineDirection) ) * lineDirection;
}

void ClusteredLights::setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera)
{
    m_ViewMatrix = camera.getView();
    m_ClusterCount = clusterCount;

    m_Clusters.clear();
    m_Clusters.reserve(clusterCount.x * clusterCount.y * clusterCount.z);
    
    m_SSBOClusterInfoData.clusters.clear();
    m_SSBOClusterInfoData.clusters.assign(clusterCount.x * clusterCount.y * clusterCount.z, SSBOCluster());
    m_SSBOClusterInfoData.xCount = clusterCount.x;
    m_SSBOClusterInfoData.yCount = clusterCount.y;
    m_SSBOClusterInfoData.zCount = clusterCount.z;

    glm::vec2 clusterSize_NDC_xy = { 2.f / clusterCount.x, 2.f / clusterCount.y };
    glm::mat4 invProjectionMatrix = glm::inverse(camera.getProjection()); // Only needed for clusters setup.

    for (unsigned int z = 0; z < clusterCount.z; ++z)
    {
        for (unsigned int y = 0; y < clusterCount.y; ++y)
        {
            for (unsigned int x = 0; x < clusterCount.x; ++x)
            {
                glm::vec4 nearBottomLeft_NDC = {
                    -1.f + x * clusterSize_NDC_xy.x,
                    -1.f + y * clusterSize_NDC_xy.y,
                    -1.f, // We'll deal with depth value later
                    1.f
                };

                glm::vec4 farTopRight_NDC = {
                    -1.f + (x + 1) * clusterSize_NDC_xy.x,
                    -1.f + (y + 1) * clusterSize_NDC_xy.y,
                    -1.f, // We'll deal with depth value later
                    1.f
                };

                glm::vec4 nearBottomLeft4_VS = invProjectionMatrix * nearBottomLeft_NDC;
                glm::vec3 nearBottomLeft_VS = nearBottomLeft4_VS / nearBottomLeft4_VS.w;
                glm::vec4 farTopRight4_VS = invProjectionMatrix * farTopRight_NDC;
                glm::vec3 farTopRight_VS = farTopRight4_VS / farTopRight4_VS.w;

                // Now we need to find the depth values of this cluster.
                float nearDepth = camera.getNear() * glm::pow(camera.getFar() / camera.getNear(), static_cast<float>(z) / clusterCount.z);
                float farDepth = camera.getNear() * glm::pow(camera.getFar() / camera.getNear(), static_cast<float>(z + 1) / clusterCount.z);

                // Now we need to find the intersection from the line eyePos -> AABB corners to near and far z planes.
                glm::vec3 nearMin = intersectionLineAndZPerpendicularPlane(
                    glm::vec3(0.f), // View origin
                    glm::vec3(nearBottomLeft_VS), // direction is origin -> corner
                    nearDepth
                );
                glm::vec3 nearMax = intersectionLineAndZPerpendicularPlane(
                    glm::vec3(0.f), // View origin
                    glm::vec3(farTopRight_VS), // direction is origin -> corner
                    nearDepth
                );
                glm::vec3 farMin = intersectionLineAndZPerpendicularPlane(
                    glm::vec3(0.f), // View origin
                    glm::vec3(nearBottomLeft_VS), // direction is origin -> corner
                    farDepth
                );
                glm::vec3 farMax = intersectionLineAndZPerpendicularPlane(
                    glm::vec3(0.f), // View origin
                    glm::vec3(farTopRight_VS), // direction is origin -> corner
                    farDepth
                );

                glm::vec3 minAABB_VS = glm::min(nearMin, farMin);
                glm::vec3 maxAABB_VS = glm::max(nearMax, farMax);

                m_Clusters.emplace_back(minAABB_VS, maxAABB_VS);
            }
        }
    }
}

void ClusteredLights::beginFrame()
{
    m_SSBOLightIndicesData.indices.clear();
    m_ClusterIndexToSSBOLightIndices.clear();

    for (size_t i = 0; i < m_Clusters.size(); ++i)
        m_ClusterIndexToSSBOLightIndices.emplace(i, std::vector<int>());
}

void ClusteredLights::submitLight(const glm::vec3& position, float radius, int SSBOIndex)
{
    glm::vec3 position_VS = glm::vec3(m_ViewMatrix * glm::vec4(position, 1.f));

    for (size_t i = 0; i < m_Clusters.size(); ++i)
    {
        if (m_Clusters[i].intersectsSphereVS(position_VS, radius).intersects)
        {
            m_ClusterIndexToSSBOLightIndices.at(i).push_back(SSBOIndex);
        }
    }
}

void ClusteredLights::endFrame()
{
    int offset = 0;
    for (size_t i = 0; i < m_Clusters.size(); ++i)
    {
        const auto& clusterLights = m_ClusterIndexToSSBOLightIndices.at(i);

        m_SSBOLightIndicesData.indices.insert(
            m_SSBOLightIndicesData.indices.end(),
            clusterLights.begin(), clusterLights.end()
        );

        int indicesCount = static_cast<int>(clusterLights.size());

        m_SSBOClusterInfoData.clusters.at(i).indexOffset = offset;
        m_SSBOClusterInfoData.clusters.at(i).indexCount = indicesCount;
        offset += indicesCount;
    }

    m_SSBOClusterInfoSSBO.setData(m_SSBOClusterInfoData);
    //LOG_TRACE("Clusters info SSBO capacity: {0}", m_SSBOClusterInfoData.getSize());

    m_SSBOLightIndicesSSBO.setData(m_SSBOLightIndicesData);
    //LOG_TRACE("Light indices SSBO capacity: {0}", m_SSBOLightIndicesSSBO.getCapacity());
}

} // namespace vrm
