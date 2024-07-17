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

#include "Vroom/Asset/AssetInstance/ComputeShaderInstance.h"

#include "Vroom/Render/Clustering/Cluster.h"
#include "Vroom/Render/RawShaderData/SSBOClusterInfo.h"

#include "Vroom/Render/Abstraction/DynamicSSBO.h"
#include "Vroom/Render/Camera/CameraBasic.h"


namespace vrm
{

class ClusteredLights
{
public:
    ClusteredLights();
    ClusteredLights(const ClusteredLights&) = default;
    ClusteredLights(ClusteredLights&&) = default;
    ~ClusteredLights() = default;

    ClusteredLights& operator=(const ClusteredLights&) = default;
    ClusteredLights& operator=(ClusteredLights&&) = default;

    void setBindingPoint(int clusterInfoBindingPoint);

    void setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera);

    void processLights(const CameraBasic& camera);

private:
    SSBOClusterInfo m_SSBOClusterInfoData;
    DynamicSSBO m_SSBOClusterInfoSSBO;

    glm::uvec3 m_ClusterCount;
    unsigned int m_TotalClusters;
    glm::mat4 m_Projection;

    ComputeShaderInstance m_ClustersBuilder, m_LightsCuller;
};

} // namespace vrm
