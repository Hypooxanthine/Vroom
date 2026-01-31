#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Abstraction/GLCore.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderViewport.h"

using namespace vrm;

RenderPass::RenderPass()
{}

RenderPass::RenderPass(const std::string& name)
{
  setName(name);
}

RenderPass::~RenderPass()
{}

void RenderPass::init()
{
  _beginDebugGroup("init");
  
  onInit();
  
  _endDebugGroup();
}

void RenderPass::setup(const RenderPassContext& ctx)
{
  _beginDebugGroup("setup");
  
  onSetup(ctx);
  
  _endDebugGroup();
}

void RenderPass::render(const RenderPassContext& ctx) const
{
  _beginDebugGroup("render");
  
  onRender(ctx);
  
  _endDebugGroup();
}

void RenderPass::cleanup(const RenderPassContext& ctx)
{
  _beginDebugGroup("cleanup");
  
  onCleanup(ctx);
  
  _endDebugGroup();
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
    key.asset   = asset;
    key.defines = customDefines;
  }
  return m_materialsRef->getMaterial(key);
}

const PassMaterial& RenderPass::getPassMaterial(MaterialAsset::Handle asset) const
{
  return getPassMaterial(asset, &m_defines);
}

void RenderPass::_beginDebugGroup(const std::string& stage) const
{
  std::string message = std::to_string(getPassIndex()) + ": " + getName() + " (" + stage + ")";

  glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, getPassIndex(), message.size(), message.c_str());
}

void RenderPass::_endDebugGroup() const
{
  glPopDebugGroup();
}
