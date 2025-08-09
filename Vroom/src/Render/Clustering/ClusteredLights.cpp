#include "Vroom/Render/Clustering/ClusteredLights.h"

#include <glm/gtx/string_cast.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Scene/Scene.h"

using namespace vrm;

namespace
{

  struct RawHeader
  {
    glm::uint xCount;
    glm::uint yCount;
    glm::uint zCount;
    glm::uint _pad;
  };

  struct RawCluster
  {
    glm::vec4 minAABB_VS;
    glm::vec4 maxAABB_VS;
    glm::uint indexCount;
    glm::uint lightIndices[100];
    glm::uint _pad[3];
  };

}

ClusteredLights::ClusteredLights()
{
  using namespace std::string_literals;

  std::string_view builderPath = "Resources/Engine/Shader/ClusteredRendering/BuildClusters_Shader.json";
  const auto &builderData = AssetManager::Get().getAsset<ShaderAsset>(builderPath.data())->getShaderData();

  auto shaderSrc = builderData.combine();
  VRM_ASSERT_MSG(
      m_ClustersBuilder.addShaderStage(gl::Shader::EShaderType::eCompute, shaderSrc.compute.sourceCode, true),
      "Could not compile {} compute shader. Error:\n{}", builderPath, m_ClustersBuilder.getError());

  VRM_ASSERT_MSG(
      m_ClustersBuilder.validate(true),
      "Could not link {} compute shader. Error:\n", builderPath, m_ClustersBuilder.getError());

  // shaderSrc.compute.dump(builderPath.data() + ".dump.glsl"s);
  // VRM_LOG_INFO("Builder dumped into {}", builderPath.data() + ".dump.glsl"s);

  std::string_view cullerPath = "Resources/Engine/Shader/ClusteredRendering/Cull_Shader.json";
  const auto &cullerData = AssetManager::Get().getAsset<ShaderAsset>(cullerPath.data())->getShaderData();

  shaderSrc = cullerData.combine();
  VRM_ASSERT_MSG(
      m_LightsCuller.addShaderStage(gl::Shader::EShaderType::eCompute, shaderSrc.compute.sourceCode, true),
      "Could not compile {} compute shader. Error:\n{}", cullerPath, m_LightsCuller.getError());

  VRM_ASSERT_MSG(
      m_LightsCuller.validate(true),
      "Could not link {} compute shader. Error:\n", cullerPath, m_LightsCuller.getError());

  // shaderSrc.compute.dump(cullerPath.data() + ".dump.glsl"s);
  // VRM_LOG_INFO("Builder dumped into {}", cullerPath.data() + ".dump.glsl"s);
}

void ClusteredLights::setupClusters(const glm::uvec3 &clusterCount, const CameraBasic &camera)
{
  if (m_ClusterCount == clusterCount && m_Projection == camera.getProjection())
    return;

  m_ClusterCount = clusterCount;
  m_TotalClusters = m_ClusterCount.x * m_ClusterCount.y * m_ClusterCount.z;
  m_Projection = camera.getProjection();

  RawHeader header;
  {
    header.xCount = clusterCount.x;
    header.yCount = clusterCount.y;
    header.zCount = clusterCount.z;
  }

  m_clustersBuffer.ensureCapacity(sizeof(header) + m_TotalClusters * sizeof(RawCluster));
  std::span<uint8_t> map = m_clustersBuffer.mapWriteOnly();
  std::memcpy(map.data(), &header, sizeof(header));
  // m_SSBOClusterInfoSSBO.setData(m_SSBOClusterInfoData.getVariableData(), m_SSBOClusterInfoData.getVariableSize(), m_SSBOClusterInfoData.getConstantSize());

  m_clustersBuffer.unmap();
}

void ClusteredLights::processLights(const CameraBasic &camera, const gl::Buffer& lights) const
{
  glm::mat4 invProjectionMatrix = glm::inverse(camera.getProjection()); // Only needed for clusters setup.

  m_ClustersBuilder.bind();
  m_ClustersBuilder.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer());
  m_ClustersBuilder.setUniform1f("u_Near", camera.getNear());
  m_ClustersBuilder.setUniform1f("u_Far", camera.getFar());
  m_ClustersBuilder.setUniformMat4f("u_InvProjection", invProjectionMatrix);
  GLCall(glDispatchCompute(m_ClusterCount.x, m_ClusterCount.y, m_ClusterCount.z));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

  m_LightsCuller.bind();
  m_LightsCuller.setStorageBuffer("PointLightBlock", lights);
  m_LightsCuller.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer());
  m_LightsCuller.setUniformMat4f("u_View", camera.getView());
  GLCall(glDispatchCompute(m_TotalClusters / 128u, 1, 1));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}
