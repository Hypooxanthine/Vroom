#include "Vroom/Render/Clustering/ClusteredLights.h"

#include <glm/gtx/string_cast.hpp>

namespace vrm
{

void ClusteredLights::setBindingPoints(int clusterInfoBindingPoint, int lightIndicesBindingPoint)
{
    m_SSBOClusterInfoSSBO.setBindingPoint(clusterInfoBindingPoint);
    m_SSBOLightIndicesSSBO.setBindingPoint(lightIndicesBindingPoint);
}

void ClusteredLights::beginFrame(const glm::uvec3& clusterCount, const CameraBasic& camera)
{
    //LOG_INFO("Beginning frame with cluster count: {0}", glm::to_string(clusterCount));
    m_Clusters.clear();
    m_SSBOClusterInfoData.clusters.clear();
    m_SSBOLightIndicesData.indices.clear();
    m_ClusterIndexToSSBOLightIndices.clear();
    m_SSBOClusterInfoData.xCount = clusterCount.x;
    m_SSBOClusterInfoData.yCount = clusterCount.y;
    m_SSBOClusterInfoData.zCount = clusterCount.z;
    m_ClusterCount = clusterCount;
    m_ViewProjectionMatrix = camera.getViewProjection();
    m_InvViewProjectionMatrix = glm::inverse(m_ViewProjectionMatrix);

    m_Clusters.reserve(m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z);
    m_SSBOClusterInfoData.clusters.assign(m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z, SSBOCluster());

    glm::vec3 clusterSize = glm::vec3(2.f, 2.f, 2.f) / glm::vec3(m_ClusterCount);

    for (unsigned int z = 0; z < m_ClusterCount.z; ++z)
    {
        for (unsigned int y = 0; y < m_ClusterCount.y; ++y)
        {
            for (unsigned int x = 0; x < m_ClusterCount.x; ++x)
            {
                glm::vec3 nearBottomLeft = { x * clusterSize.x - 1.f, y * clusterSize.y - 1.f, z * clusterSize.z - 1.f };
                glm::vec3 farTopRight = { (x + 1) * clusterSize.x - 1.f, (y + 1) * clusterSize.y - 1.f, (z + 1) * clusterSize.z - 1.f };
                m_Clusters.emplace_back(nearBottomLeft, farTopRight, m_InvViewProjectionMatrix, camera.getNear(), camera.getFar());
                m_Clusters.back().setupFastSphereIntersectionWS();
                m_ClusterIndexToSSBOLightIndices[m_Clusters.size() - 1] = std::vector<int>();
            }
        }
    }
}

const std::vector<Cluster>& ClusteredLights::getClusters() const
{
    return m_Clusters;
}

const Cluster& ClusteredLights::getCluster(unsigned int x, unsigned int y, unsigned int z) const
{
    return m_Clusters[z * m_ClusterCount.y * m_ClusterCount.x + y * m_ClusterCount.x + x];
}

void ClusteredLights::submitLight(const glm::vec3& position, float radius, int SSBOIndex)
{
    //LOG_TRACE("Submitting light at position: {0}, radius: {1}. SSBO index : {2}", glm::to_string(position), radius, SSBOIndex);
    // Bruteforcing it for now. Later, we can find one cluster that contains the light in NDC space, then check check clusters around it.
    /// @todo Optimize this.
    for (size_t i = 0; i < m_Clusters.size(); ++i)
    {
        if (m_Clusters[i].intersectsSphereWSFast(position, radius))
        {
            size_t x = i % m_ClusterCount.x;
            size_t y = (i / m_ClusterCount.x) % m_ClusterCount.y;
            size_t z = i / (m_ClusterCount.x * m_ClusterCount.y);
            //LOG_TRACE("Light intersects cluster at x: {0}, y: {1}, z: {2}", x, y, z);
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
