#pragma once

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Render/Abstraction/VertexArray.h"

namespace vrm::gl
{
  class FrameBuffer;
  class Texture;
} // namespace vrm::gl

namespace vrm
{
  
  class GaussianBlurPass : public RenderPass
  {
  public:

    gl::FrameBuffer* framebufferA = nullptr;
    gl::FrameBuffer* framebufferB = nullptr;

    gl::Texture* texture = nullptr; // Input of A and output of the pass
    gl::Texture* intermediateTexture = nullptr; // Input of B

    size_t blurPasses = 5;

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

