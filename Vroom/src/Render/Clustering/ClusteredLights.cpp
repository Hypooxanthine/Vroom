#include "Vroom/Render/Clustering/ClusteredLights.h"

namespace vrm
{

ClusteredLights::ClusteredLights(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::uvec3& clusterCount)
{
    setupClusters(nearBottomLeft, farTopRight, clusterCount.x, clusterCount.y, clusterCount.z);
}

void ClusteredLights::setupClusters(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, unsigned int xCount, unsigned int yCount, unsigned int zCount)
{
    m_Clusters.clear();
    m_ClusterData.clear();

    m_Clusters.reserve(xCount * yCount * zCount);
    m_ClusterData.reserve(xCount * yCount * zCount);

    glm::vec3 clusterSize = (farTopRight - nearBottomLeft) / glm::vec3(xCount, yCount, zCount);

    for (unsigned int z = 0; z < zCount; ++z)
    {
        for (unsigned int y = 0; y < yCount; ++y)
        {
            for (unsigned int x = 0; x < xCount; ++x)
            {
                glm::vec3 clusterNearBottomLeft = nearBottomLeft + glm::vec3(x, y, z) * clusterSize;
                glm::vec3 clusterFarTopRight = clusterNearBottomLeft + clusterSize;

                Cluster cluster(clusterNearBottomLeft, clusterFarTopRight);
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
