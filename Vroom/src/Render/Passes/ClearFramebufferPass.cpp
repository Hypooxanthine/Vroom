#include "Vroom/Render/Passes/ClearFrameBufferPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

ClearFrameBufferPass::ClearFrameBufferPass()
  : RenderPass()
{

}

ClearFrameBufferPass::~ClearFrameBufferPass()
{

}

void ClearFrameBufferPass::onRender(const RenderPassContext& ctx) const
{
  VRM_ASSERT_MSG(framebuffer != nullptr, "Invalid source framebuffer");
  VRM_ASSERT_MSG(framebuffer->isCreated(), "Source framebuffer is not created");
  
  framebuffer->clearColors();
  framebuffer->clearDepth();
}
