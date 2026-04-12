#pragma once

#include "Rasterizer/Buffer.h"
#include "Rasterizer/VertexArray.h"
#include "RenderObjects/fwds.h"
#include "Renderer/RenderPass.h"

namespace vrm
{

namespace gl
{
class FrameBuffer;
}

class RenderSkyboxPass : public RenderPass
{
public:

  RenderSkyboxPass(const std::string& name = "RenderSkyboxPass");
  ~RenderSkyboxPass();

public:

  const gl::FrameBuffer*  framebuffer = nullptr;
  const render::Cubemap** skybox      = nullptr;

protected:

  virtual void onInit() override;

  virtual void onSetup(const RenderPassContext& ctx) override;

  virtual void onRender(const RenderPassContext& ctx) const override;

private:

  gl::Buffer      m_vbo;
  gl::VertexArray m_vao;

  const PassMaterial* m_material = nullptr;
};

} // namespace vrm
