#include "Vroom/Render/Passes/ToneMappingPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

ToneMappingPass::ToneMappingPass(const std::string& name) : RenderPass(name) {}

ToneMappingPass::~ToneMappingPass() {}

void ToneMappingPass::onInit()
{
  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");
  VRM_ASSERT_MSG(hdrTex != nullptr, "Invalid hdr texture");

  m_matAsset = AssetManager::Get().getAsset<MaterialAsset>(
    "Resources/Engine/Material/ToneMappingPostProcessMaterial.json");
  m_material = &getPassMaterial(m_matAsset);

  m_dummyVao.create();
}

void ToneMappingPass::onSetup(const RenderPassContext& ctx) {}

void ToneMappingPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("ToneMappingPass::onRender");

  auto& shader = m_material->getShader();
  shader.bind();
  shader.setTexture("u_hdrTex", *hdrTex, 0);
  if (blurredBrightness) // Bloom activated
  {
    shader.setTexture("u_blurredBrightness", *blurredBrightness, 1);
    shader.setUniform1f("u_bloomIntensity",
                        ctx.dynamicSettings->bloom.intensity);
  }
  shader.setUniform1f("u_exposure", ctx.dynamicSettings->hdr.exposure);
  framebufferTarget->bind();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glViewport(0, 0, framebufferTarget->getWidth(),
             framebufferTarget->getHeight());

  gl::VertexArray::Bind(m_dummyVao);
  gl::Buffer::Unbind(GL_ARRAY_BUFFER);
  gl::Buffer::Unbind(GL_ELEMENT_ARRAY_BUFFER);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
