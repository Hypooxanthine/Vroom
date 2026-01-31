#include "Vroom/Render/Passes/BlitFrameBufferPass.h"

#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

BlitFrameBufferPass::BlitFrameBufferPass(const std::string& name) : RenderPass(name) {}

BlitFrameBufferPass::~BlitFrameBufferPass() {}

void BlitFrameBufferPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("BlitFrameBufferPass::onRender");

  VRM_ASSERT_MSG(source != nullptr, "Invalid source framebuffer");
  VRM_ASSERT_MSG(destination != nullptr, "Invalid destination framebuffer");
  VRM_ASSERT_MSG(source->isCreated(), "Source framebuffer is not created");
  VRM_ASSERT_MSG(destination->isCreated(),
                 "Destination framebuffer is not created");

  gl::FrameBuffer::Blit(*destination, *source);
}
