#include "Vroom/Render/Clustering/ClusteredLights.h"

namespace vrm
{

ClusteredLights::ClusteredLights(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::uvec3& clusterCount, const glm::mat4& viewProjectionMatrix)
{
    setupClusters(nearBottomLeft, farTopRight, clusterCount, viewProjectionMatrix);
}

void ClusteredLights::setupClusters(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::uvec3& clusterCount, const glm::mat4& viewProjectionMatrix)
{
    m_Clusters.clear();
    m_ClusterData.clear();

    m_ClusterCount = clusterCount;

    m_Clusters.reserve(m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z);
    m_ClusterData.reserve(m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z);

    glm::vec3 clusterSize = (farTopRight - nearBottomLeft) / glm::vec3(m_ClusterCount);

    for (unsigned int z = 0; z < m_ClusterCount.z; ++z)
    {
        for (unsigned int y = 0; y < m_ClusterCount.y; ++y)
        {
            for (unsigned int x = 0; x < m_ClusterCount.z; ++x)
            {
                glm::vec3 clusterNearBottomLeft = nearBottomLeft + glm::vec3(x, y, z) * clusterSize;
                glm::vec3 clusterFarTopRight = clusterNearBottomLeft + clusterSize;

                Cluster cluster(clusterNearBottomLeft, clusterFarTopRight, viewProjectionMatrix);
                m_Clusters.push_back(cluster);

                SSBOClusterData clusterData = { clusterNearBottomLeft, clusterFarTopRight };
                m_ClusterData.push_back(clusterData);
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

const SSBOClusterData* ClusteredLights::getClusterData() const
{
    return m_ClusterData.data();
}

} // namespace vrm
