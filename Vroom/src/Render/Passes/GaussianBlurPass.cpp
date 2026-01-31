#include "Vroom/Render/Passes/GaussianBlurPass.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Passes/RenderPass.h"

using namespace vrm;

GaussianBlurPass::GaussianBlurPass(const std::string& name)
  : RenderPass(name)
{}

void GaussianBlurPass::onInit()
{
  VRM_ASSERT_MSG(framebufferA != nullptr, "Invalid framebuffer A");
  VRM_ASSERT_MSG(framebufferB != nullptr, "Invalid framebuffer B");
  VRM_ASSERT_MSG(texture != nullptr, "Invalid texture");
  VRM_ASSERT_MSG(intermediateTexture != nullptr, "Invalid intermediateTexture");

  m_matAsset = AssetManager::Get().getAsset<MaterialAsset>(
    "Resources/Engine/Material/GaussianBlurPostProcessMaterial.json");
  m_material = &getPassMaterial(m_matAsset);

  m_dummyVao.create();
}

void GaussianBlurPass::onSetup(const RenderPassContext& ctx) {}

void GaussianBlurPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("GaussianBlurPass::onRender");

  const gl::FrameBuffer*  fb[]          = { framebufferA, framebufferB };
  const gl::Texture*      input[]       = { texture, intermediateTexture };
  static constexpr GLuint verticalVal[] = { 0, 1 };

  auto& shader = m_material->getShader();
  shader.bind();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glViewport(0, 0, framebufferA->getWidth(), framebufferA->getHeight());

  gl::VertexArray::Bind(m_dummyVao);
  gl::Buffer::Unbind(GL_ARRAY_BUFFER);
  gl::Buffer::Unbind(GL_ELEMENT_ARRAY_BUFFER);

  for (uint16_t i = 0; i < uint16_t(ctx.dynamicSettings->bloom.blurPasses) * 2;
       ++i)
  {
    uint16_t index = i % 2;

    fb[index]->bind();

    shader.setTexture("u_inputTexture", *input[index], index);
    shader.setUniform1ui("u_vertical", verticalVal[index]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}
