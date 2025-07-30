#pragma once

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

namespace vrm
{

  class RenderSkybox;

  namespace gl
  {
    class FrameBuffer;
  }
  
  class RenderSkyboxPass : public RenderPass
  {
  public:
    
    RenderSkyboxPass();
    ~RenderSkyboxPass();

  public:

    const gl::FrameBuffer* framebuffer = nullptr;
    const RenderSkybox* skybox = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;
  
  private:

    gl::Buffer m_vbo;
    gl::VertexArray m_vao;

    const PassMaterial* m_material = nullptr;
  
  };

} // namespace vrm
