#include "Vroom/Render/Passes/LightClusteringPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Render/RenderView.h"
#include "Vroom/Render/Clustering/LightRegistry.h"

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

LightClusteringPass::LightClusteringPass()
  : RenderPass()
{

}

LightClusteringPass::~LightClusteringPass()
{

}

void LightClusteringPass::onInit()
{
  m_setupClustersMat = &getPassMaterial(AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/BuildClustersMaterial.json"));
  m_cullLightsMat    = &getPassMaterial(AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/CullLightsMaterial.json"));

  VRM_CHECK_RET_MSG(m_setupClustersMat != nullptr, "Invalid setup cluster material");
  VRM_CHECK_RET_MSG(m_cullLightsMat != nullptr, "Invalid cull lights material");
}

void LightClusteringPass::onSetup(const RenderPassContext& ctx)
{
  m_totalClusters = clusterCount.x * clusterCount.y * clusterCount.z;

  RawHeader header;
  {
    header.xCount = clusterCount.x;
    header.yCount = clusterCount.y;
    header.zCount = clusterCount.z;
  }

  m_clustersBuffer.ensureCapacity(sizeof(header) + m_totalClusters * sizeof(RawCluster));
  std::span<uint8_t> map = m_clustersBuffer.mapWriteOnly();
  std::memcpy(map.data(), &header, sizeof(header));

  m_clustersBuffer.unmap();
}

void LightClusteringPass::onRender(const RenderPassContext& ctx) const
{
  if (m_setupClustersMat == nullptr || m_cullLightsMat == nullptr)
  {
    return;
  }

  VRM_ASSERT_MSG(lightsStorageBuffer != nullptr, "Invalid lightsStorageBuffer");
  VRM_ASSERT_MSG(ctx.views.size() > 0, "Invalid camera");

  const CameraBasic& camera = *ctx.views.front().getCamera();

  glm::mat4 invProjectionMatrix = glm::inverse(camera.getProjection()); // Only needed for clusters setup.

  const gl::Shader& setupClustersShader = m_setupClustersMat->getShader();
  setupClustersShader.bind();
  setupClustersShader.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer());
  setupClustersShader.setUniform1f("u_Near", camera.getNear());
  setupClustersShader.setUniform1f("u_Far", camera.getFar());
  setupClustersShader.setUniformMat4f("u_InvProjection", invProjectionMatrix);
  GLCall(glDispatchCompute(clusterCount.x, clusterCount.y, clusterCount.z));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

  const gl::Shader& cullLightsShader = m_cullLightsMat->getShader();
  cullLightsShader.bind();
  cullLightsShader.setStorageBuffer("PointLightBlock", *lightsStorageBuffer);
  cullLightsShader.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer());
  cullLightsShader.setUniformMat4f("u_View", camera.getView());
  GLCall(glDispatchCompute(m_totalClusters / 128u, 1, 1));
  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}
