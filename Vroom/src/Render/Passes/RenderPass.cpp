#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/Abstraction/Shader.h"

using namespace vrm;

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::init()
{
  onInit();
}

void RenderPass::setup(const RenderPassContext& ctx)
{
  onSetup(ctx);
}

void RenderPass::render(const RenderPassContext& ctx) const
{
  onRender(ctx);
}

void RenderPass::cleanup(const RenderPassContext& ctx)
{
  onCleanup(ctx);
}

void RenderPass::applyCameraUniforms(const gl::Shader& shader, const CameraBasic& camera)
{
  shader.setUniformMat4f("u_View", camera.getView());
  shader.setUniformMat4f("u_Projection", camera.getProjection());
  shader.setUniformMat4f("u_ViewProjection", camera.getViewProjection());
  shader.setUniform3f("u_ViewPosition", camera.getPosition());
  shader.setUniform1f("u_Near", camera.getNear());
  shader.setUniform1f("u_Far", camera.getFar());
}

void RenderPass::applyViewportUniforms(const gl::Shader& shader, const render::Viewport& viewport)
{
  shader.setUniform2ui("u_ViewportOrigin", viewport.getOrigin().x, viewport.getOrigin().y);
  shader.setUniform2ui("u_ViewportSize", viewport.getSize().x, viewport.getSize().y);
}

const PassMaterial& RenderPass::getPassMaterial(MaterialAsset::Handle asset, const MaterialDefines* customDefines) const
{
  PassMaterials::Key key;
  {
    key.asset = asset;
    key.defines = customDefines;
  }
  return m_materialsRef->getMaterial(key);
}

const PassMaterial& RenderPass::getPassMaterial(MaterialAsset::Handle asset) const
{
  return getPassMaterial(asset, &m_defines);
}
