#pragma once

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Render/Abstraction/VertexArray.h"

namespace vrm::gl
{
  class FrameBuffer;
  class Texture;
}

namespace vrm
{
  
  class ToneMappingPass : public RenderPass
  {
  public:
    
    ToneMappingPass();
    virtual ~ToneMappingPass();

  public:

    gl::FrameBuffer* framebufferTarget = nullptr;
    gl::Texture* hdrTex = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;
  
  private:

    MaterialAsset::Handle m_matAsset = {};
    const PassMaterial* m_material = {};
    gl::VertexArray m_dummyVao = {};
  };

} // namespace vrm 
