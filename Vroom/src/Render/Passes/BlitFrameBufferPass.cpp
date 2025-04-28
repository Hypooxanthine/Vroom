#include "Vroom/Render/Passes/BlitFrameBufferPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

BlitFrameBufferPass::BlitFrameBufferPass()
  : RenderPass()
{

}

BlitFrameBufferPass::~BlitFrameBufferPass()
{

}

void BlitFrameBufferPass::onRender(const RenderPassContext& ctx) const
{
  VRM_ASSERT_MSG(source != nullptr, "Invalid source framebuffer");
  VRM_ASSERT_MSG(destination != nullptr, "Invalid destination framebuffer");
  VRM_ASSERT_MSG(source->isCreated(), "Source framebuffer is not created");
  VRM_ASSERT_MSG(destination->isCreated(), "Destination framebuffer is not created");

  gl::FrameBuffer::Blit(*destination, *source);
}
