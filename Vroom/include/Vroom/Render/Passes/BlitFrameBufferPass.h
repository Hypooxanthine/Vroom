#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

namespace vrm::gl
{
  class FrameBuffer;
}

namespace vrm
{

  class BlitFrameBufferPass : public RenderPass
  {
  public:

    BlitFrameBufferPass();
    virtual ~BlitFrameBufferPass();

  public:

    gl::FrameBuffer* source = nullptr;
    gl::FrameBuffer* destination = nullptr;

  protected:

    virtual void onRender(const RenderPassContext& ctx) const override;
  };

} // namespace vrm
