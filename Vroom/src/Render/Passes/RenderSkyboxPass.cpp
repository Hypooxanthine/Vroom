#include "Vroom/Render/Passes/RenderSkyboxPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

#include "Vroom/Render/RenderObject/RenderSkybox.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Camera/CameraBasic.h"

using namespace vrm;

RenderSkyboxPass::RenderSkyboxPass()
{

}

RenderSkyboxPass::~RenderSkyboxPass()
{

}

void RenderSkyboxPass::onInit()
{
  VRM_CHECK_RET_MSG(skybox != nullptr, "Skybox is not valid");

  auto matAsset = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/SkyboxMaterial.json");
  VRM_CHECK_RET_MSG(matAsset.isValid(), "Could not load skybox material");

  m_material = &getPassMaterial(matAsset);
  VRM_CHECK_RET_MSG(m_material, "Could not get skybox pass material");

  auto meshAsset = AssetManager::Get().getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj");
  VRM_CHECK_RET_MSG(meshAsset.isValid(), "Could not load skybox cube");
}

void RenderSkyboxPass::onSetup(const RenderPassContext& ctx)
{

}

void RenderSkyboxPass::onRender(const RenderPassContext& ctx) const
{
  if (!m_material || !skybox->getCubemap().isValid())
    return;

  ctx.frameBufferTarget->bind();

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW); // From inside the cube
  glCullFace(GL_BACK);
  
  const auto& shader = m_material->getShader();
  shader.bind();
  shader.setTexture("u_cubemap", skybox->getCubemap()->getGpuTexture(), 0);
  shader.setUniformMat4f("u_viewProjection", ctx.mainCamera->getViewProjection());

  for (const auto& submesh : m_cube->getSubMeshes())
  {
    gl::VertexArray::Bind(submesh.renderMesh.getVertexArray());
    glDrawElements(GL_TRIANGLES, submesh.renderMesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}
