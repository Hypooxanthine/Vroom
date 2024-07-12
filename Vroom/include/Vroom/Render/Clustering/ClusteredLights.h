#pragma once

/**
 * @file ClusteredLights.h
 * @author Alexandre Beaujon
 * @brief With the great help of https://github.com/DaveH355/clustered-shading .
 * @date 2024-07-12
 * 
 */

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

    void setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera);

    void setBindingPoints(int clusterInfoBindingPoint, int lightIndicesBindingPoint);

    void beginFrame();

    void submitLight(const glm::vec3& position, float radius, int SSBOIndex);

    void endFrame();

private:
    std::vector<Cluster> m_Clusters;
    std::unordered_map<size_t, std::vector<int>> m_ClusterIndexToSSBOLightIndices;
    SSBOClusterInfo m_SSBOClusterInfoData;
    SSBOLightIndices m_SSBOLightIndicesData;
    DynamicSSBO m_SSBOClusterInfoSSBO, m_SSBOLightIndicesSSBO;

    glm::uvec3 m_ClusterCount;
    glm::mat4 m_ViewMatrix; // So that we can convert a light center from WS to VS.
};

} // namespace vrm
