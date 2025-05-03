#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

namespace vrm::gl
{
  class FrameBuffer;
}

namespace vrm
{

  class ClearFrameBufferPass : public RenderPass
  {
  public:

    ClearFrameBufferPass();
    virtual ~ClearFrameBufferPass();

  public:

    gl::FrameBuffer* framebuffer = nullptr;

  protected:

    virtual void onRender(const RenderPassContext& ctx) const override;
  };

} // namespace vrm
