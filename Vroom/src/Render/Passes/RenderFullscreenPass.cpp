#include "Vroom/Render/Passes/RenderFullscreenPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

RenderFullscreenPass::RenderFullscreenPass()
{

}

RenderFullscreenPass::~RenderFullscreenPass()
{

}

void RenderFullscreenPass::onInit()
{
  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");

  // Vertex shader will draw vertices directly in clip space for fullscreen
  addDefine("VRM_RENDER_FULLSCREEN");
}

void RenderFullscreenPass::onSetup(const RenderPassContext& ctx)
{
  
}

void RenderFullscreenPass::onRender(const RenderPassContext& ctx) const
{
  const PassMaterial& mat = getPassMaterial(material);
  mat.getShader().bind();

  glDrawArrays(GL_TRIANGLES, 0, 3);
}
