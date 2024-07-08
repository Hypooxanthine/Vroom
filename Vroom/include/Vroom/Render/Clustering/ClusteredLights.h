#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Vroom/Render/Clustering/Cluster.h"
#include "Vroom/Render/RawShaderData/SSBOClusterInfo.h"
#include "Vroom/Render/RawShaderData/SSBOLightIndices.h"

#include "Vroom/Render/Abstraction/DynamicSSBO.h"
#include "Vroom/Render/Camera/CameraBasic.h"


namespace vrm
{

class ClusteredLights
{
public:
    ClusteredLights() = default;
    ClusteredLights(const ClusteredLights&) = default;
    ClusteredLights(ClusteredLights&&) = default;
    ~ClusteredLights() = default;

    ClusteredLights& operator=(const ClusteredLights&) = default;
    ClusteredLights& operator=(ClusteredLights&&) = default;

    void setBindingPoints(int clusterInfoBindingPoint, int lightIndicesBindingPoint);

    void beginFrame(const glm::uvec3& clusterCount, const CameraBasic& camera);
    const std::vector<Cluster>& getClusters() const;
    const Cluster& getCluster(unsigned int x, unsigned int y, unsigned int z) const;

    void submitLight(const glm::vec3& position, float radius, int SSBOIndex);

    void endFrame();

private:
    std::vector<Cluster> m_Clusters;
    std::unordered_map<size_t, std::vector<int>> m_ClusterIndexToSSBOLightIndices;
    SSBOClusterInfo m_SSBOClusterInfoData;
    SSBOLightIndices m_SSBOLightIndicesData;
    DynamicSSBO m_SSBOClusterInfoSSBO, m_SSBOLightIndicesSSBO;

    glm::uvec3 m_ClusterCount;
    glm::mat4 m_ViewProjectionMatrix, m_InvViewProjectionMatrix;
};

} // namespace vrm
