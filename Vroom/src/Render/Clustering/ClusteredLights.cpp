#include "Vroom/Render/Clustering/ClusteredLights.h"

#include <glm/gtx/string_cast.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Scene/Scene.h"

using namespace vrm;

ClusteredLights::ClusteredLights()
{
  using namespace std::string_literals;

  std::string_view builderPath = "Resources/Engine/Shader/ClusteredRendering/BuildClusters_Shader.json";
  const auto& builderData = AssetManager::Get().getAsset<ShaderAsset>(builderPath.data()).getStaticAsset()->getShaderData();

  auto shaderSrc = builderData.combine();
  VRM_ASSERT_MSG(
    m_ClustersBuilder.addShaderStage(Shader::EShaderType::eCompute, shaderSrc.compute.sourceCode, true),
    "Could not compile {} compute shader. Error:\n{}", builderPath, m_ClustersBuilder.getError()
  );

  VRM_ASSERT_MSG(
    m_ClustersBuilder.validate(true),
    "Could not link {} compute shader. Error:\n", builderPath, m_ClustersBuilder.getError()
  );

  // shaderSrc.compute.dump(builderPath.data() + ".dump.glsl"s);
  // VRM_LOG_INFO("Builder dumped into {}", builderPath.data() + ".dump.glsl"s);

  std::string_view cullerPath = "Resources/Engine/Shader/ClusteredRendering/Cull_Shader.json";
  const auto& cullerData = AssetManager::Get().getAsset<ShaderAsset>(cullerPath.data()).getStaticAsset()->getShaderData();

  shaderSrc = cullerData.combine();
  VRM_ASSERT_MSG(
    m_LightsCuller.addShaderStage(Shader::EShaderType::eCompute, shaderSrc.compute.sourceCode, true),
    "Could not compile {} compute shader. Error:\n{}", cullerPath, m_LightsCuller.getError()
  );

  VRM_ASSERT_MSG(
    m_LightsCuller.validate(true),
    "Could not link {} compute shader. Error:\n", cullerPath, m_LightsCuller.getError()
  );

  // shaderSrc.compute.dump(cullerPath.data() + ".dump.glsl"s);
  // VRM_LOG_INFO("Builder dumped into {}", cullerPath.data() + ".dump.glsl"s);
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

  m_ClustersBuilder.bind();
  m_ClustersBuilder.setUniform1f("u_Near", camera.getNear());
  m_ClustersBuilder.setUniform1f("u_Far", camera.getFar());
  m_ClustersBuilder.setUniformMat4f("u_InvProjection", invProjectionMatrix);
  GLCall(glDispatchCompute(m_ClusterCount.x, m_ClusterCount.y, m_ClusterCount.z));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

void ClusteredLights::processLights(const CameraBasic& camera)
{
  m_LightsCuller.bind();
  m_LightsCuller.setUniformMat4f("u_View", camera.getView());
  GLCall(glDispatchCompute(m_TotalClusters / 128u, 1, 1));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}
