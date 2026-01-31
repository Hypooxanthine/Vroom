#include "Vroom/Render/Passes/ClearFrameBufferPass.h"

#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

ClearFrameBufferPass::ClearFrameBufferPass(const std::string& name) : RenderPass(name)
{}

ClearFrameBufferPass::~ClearFrameBufferPass()
{}

void ClearFrameBufferPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("ClearFrameBufferPass::onRender");

  VRM_ASSERT_MSG(framebuffer != nullptr, "Invalid source framebuffer");
  VRM_ASSERT_MSG(framebuffer->isCreated(), "Source framebuffer is not created");

  framebuffer->clearColors();
  framebuffer->clearDepth();
}
