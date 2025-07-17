#pragma once

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm::gl
{
  class FrameBuffer;
}

namespace vrm
{
  
  class RenderFullscreenPass : public RenderPass
  {
  public:
    
    RenderFullscreenPass();
    virtual ~RenderFullscreenPass();

  public:

    gl::FrameBuffer* framebufferTarget = nullptr;
    MaterialAsset::Handle material = {};

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;
  };

} // namespace vrm 
