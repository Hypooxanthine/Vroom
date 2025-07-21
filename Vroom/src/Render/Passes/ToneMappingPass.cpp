#include "Vroom/Render/Passes/ToneMappingPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm;

ToneMappingPass::ToneMappingPass()
{

}

ToneMappingPass::~ToneMappingPass()
{

}

void ToneMappingPass::onInit()
{
  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");
  VRM_ASSERT_MSG(hdrTex != nullptr, "Invalid hdr texture");

  m_matAsset = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/ToneMappingPostProcessMaterial.json");
  m_material = &getPassMaterial(m_matAsset);

  m_dummyVao.create();
}

void ToneMappingPass::onSetup(const RenderPassContext& ctx)
{
  
}

void ToneMappingPass::onRender(const RenderPassContext& ctx) const
{
  auto& shader = m_material->getShader();
  shader.bind();
  shader.setTexture("u_hdrTex", *hdrTex, 0);
  shader.setUniform1f("u_exposure", ctx.dynamicSettings->hdr.exposure);
  framebufferTarget->bind();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glViewport(0, 0, framebufferTarget->getWidth(), framebufferTarget->getHeight());

  gl::VertexArray::Bind(m_dummyVao);
  gl::Buffer::Unbind(GL_ARRAY_BUFFER);
  gl::Buffer::Unbind(GL_ELEMENT_ARRAY_BUFFER);
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
