#include "Vroom/Render/Passes/LightClusteringPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "glm/ext/vector_uint3.hpp"
#include "glm/vector_relational.hpp"

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
    glm::uint lightIndices[103];
  };

}

LightClusteringPass::LightClusteringPass()
  : RenderPass()
{

}

LightClusteringPass::~LightClusteringPass()
{

}

size_t LightClusteringPass::getPerViewSize() const
{
  return sizeof(RawHeader) + clusterCount.x * clusterCount.y * clusterCount.z * sizeof(RawCluster);
}

void LightClusteringPass::onInit()
{
  VRM_CHECK_RET_MSG(glm::all(glm::greaterThan(clusterCount, glm::uvec3(0))), "At least one cluster is needed for each dimension");

  m_totalClusters = clusterCount.x * clusterCount.y * clusterCount.z;

  addDefine("BUILDER_LOCAL_SIZE_X", s_setupClustersLocalSize.x);
  addDefine("BUILDER_LOCAL_SIZE_Y", s_setupClustersLocalSize.y);
  addDefine("BUILDER_LOCAL_SIZE_Z", s_setupClustersLocalSize.z);

  m_setupClustersMat = &getPassMaterial(AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/BuildClustersMaterial.json"));
  m_cullLightsMat    = &getPassMaterial(AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/CullLightsMaterial.json"));

  VRM_CHECK_RET_MSG(m_setupClustersMat != nullptr, "Invalid setup cluster material");
  VRM_CHECK_RET_MSG(m_cullLightsMat != nullptr, "Invalid cull lights material");
}

void LightClusteringPass::onSetup(const RenderPassContext& ctx)
{
  if (m_totalClusters == 0)
    return;

  RawHeader header;
  {
    header.xCount = clusterCount.x;
    header.yCount = clusterCount.y;
    header.zCount = clusterCount.z;
  }

  size_t sizePerView = getPerViewSize();

  m_clustersBuffer.ensureCapacity(sizePerView * ctx.views.size());
  std::span<uint8_t> map = m_clustersBuffer.mapWriteOnly();

  // One header per view
  // It is the same one for all, but will be easier when binding a subrange of buffer later
  for (size_t i = 0; i < ctx.views.size(); ++i)
    std::memcpy(map.data() + sizePerView * i, &header, sizeof(header));

  m_clustersBuffer.unmap();

  const gl::Shader& setupClustersShader = m_setupClustersMat->getShader();
  setupClustersShader.bind();

  glm::uvec3 dispatch = (clusterCount + s_setupClustersLocalSize - glm::uvec3(1)) / s_setupClustersLocalSize;

  for (size_t i = 0; i < ctx.views.size(); ++i)
  {
    const CameraBasic& camera = *ctx.views.at(i).getCamera();

    setupClustersShader.setUniform1f("u_Near", camera.getNear());
    setupClustersShader.setUniform1f("u_Far", camera.getFar());
    setupClustersShader.setUniformMat4f("u_InvProjection", glm::inverse(camera.getProjection()));
    setupClustersShader.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer(), sizePerView * i, sizePerView);

    GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));
  }

  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}

void LightClusteringPass::onRender(const RenderPassContext& ctx) const
{
  if (m_setupClustersMat == nullptr || m_cullLightsMat == nullptr || m_totalClusters == 0)
  {
    return;
  }

  VRM_ASSERT_MSG(lightsStorageBuffer != nullptr, "Invalid lightsStorageBuffer");
  VRM_ASSERT_MSG(ctx.views.size() > 0, "Invalid camera");

  size_t sizePerView = getPerViewSize();

  const gl::Shader& cullLightsShader = m_cullLightsMat->getShader();
  cullLightsShader.bind();
  cullLightsShader.setStorageBuffer("PointLightBlock", *lightsStorageBuffer);

  glm::uvec3 dispatch = glm::uvec3((m_totalClusters + 63u) / 64u, 1, 1);

  for (size_t i = 0; i < ctx.views.size(); ++i)
  {
    const CameraBasic& camera = *ctx.views.at(i).getCamera();

    cullLightsShader.setUniformMat4f("u_View", camera.getView());
    cullLightsShader.setStorageBuffer("ClusterInfoBlock", m_clustersBuffer.getBuffer(), sizePerView * i, sizePerView);

    GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));
  }

  GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
}
