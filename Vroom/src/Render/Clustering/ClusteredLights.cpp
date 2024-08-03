#include "Vroom/Render/Clustering/ClusteredLights.h"

#include <glm/gtx/string_cast.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ComputeShaderAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Scene/Scene.h"

namespace vrm
{

ClusteredLights::ClusteredLights()
{
    m_ClustersBuilder = AssetManager::Get().getAsset<ComputeShaderAsset>("Resources/Engine/Shader/ComputeShader/ClusterGridCompute.glsl");
    m_LightsCuller = AssetManager::Get().getAsset<ComputeShaderAsset>("Resources/Engine/Shader/ComputeShader/ClusterCullingCompute.glsl");
}

void ClusteredLights::setBindingPoint(int clusterInfoBindingPoint)
{
    m_SSBOClusterInfoSSBO.setBindingPoint(clusterInfoBindingPoint);
}

void ClusteredLights::setupClusters(const glm::uvec3& clusterCount, const CameraBasic& camera)
{
    if (m_ClusterCount == clusterCount && m_Projection == camera.getProjection())
        return;

    m_ClusterCount = clusterCount;
    m_TotalClusters = m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z;
    m_Projection = camera.getProjection();
    
    m_SSBOClusterInfoData.clusters.clear();
    m_SSBOClusterInfoData.clusters.assign(clusterCount.x * clusterCount.y * clusterCount.z, SSBOCluster());
    m_SSBOClusterInfoData.xCount = clusterCount.x;
    m_SSBOClusterInfoData.yCount = clusterCount.y;
    m_SSBOClusterInfoData.zCount = clusterCount.z;
    m_SSBOClusterInfoSSBO.setData(m_SSBOClusterInfoData);

    glm::mat4 invProjectionMatrix = glm::inverse(camera.getProjection()); // Only needed for clusters setup.

    const auto& computeShader = m_ClustersBuilder.getStaticAsset()->getComputeShader();
    computeShader.bind();
    computeShader.setUniform1f("u_Near", camera.getNear());
    computeShader.setUniform1f("u_Far", camera.getFar());
    computeShader.setUniformMat4f("u_InvProjection", invProjectionMatrix);
    computeShader.dispatchCustomBarrier(m_ClusterCount.x, m_ClusterCount.y, m_ClusterCount.z, GL_SHADER_STORAGE_BARRIER_BIT);
}

void ClusteredLights::processLights(const CameraBasic& camera)
{
    const auto& computeShader = m_LightsCuller.getStaticAsset()->getComputeShader();
    computeShader.bind();
    computeShader.setUniformMat4f("u_View", camera.getView());
    // Local sise is 128 for x in the compute shader, so we need to divide by 128.
    computeShader.dispatchCustomBarrier(m_TotalClusters / 128u, 1, 1, GL_SHADER_STORAGE_BARRIER_BIT);
}

} // namespace vrm
