#pragma once

#include <vector>

#include "Vroom/Render/Clustering/Cluster.h"
#include "Vroom/Render/RawShaderData/SSBOClusterData.h"

namespace vrm
{

struct SSBOPointLightData;

class ClusteredLights
{
public:
    ClusteredLights() = default;
    ClusteredLights(const ClusteredLights&) = default;
    ClusteredLights(ClusteredLights&&) = default;
    ~ClusteredLights() = default;

    ClusteredLights& operator=(const ClusteredLights&) = default;
    ClusteredLights& operator=(ClusteredLights&&) = default;
    
    ClusteredLights(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, const glm::uvec3& clusterCount);

    void setupClusters(const glm::vec3& nearBottomLeft, const glm::vec3& farTopRight, unsigned int xCount, unsigned int yCount, unsigned int zCount);
    const std::vector<Cluster>& getClusters() const;
    const Cluster& getCluster(unsigned int x, unsigned int y, unsigned int z) const;

    const SSBOClusterData* getClusterData() const;

    void submitPointLights(const std::vector<SSBOPointLightData>& pointLights);

private:
    std::vector<Cluster> m_Clusters;
    std::vector<SSBOClusterData> m_ClusterData;

    glm::uvec3 m_ClusterCount;
};

} // namespace vrm
