#include "Vroom/Render/Passes/DrawScenePass.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"
#include "Vroom/Render/RenderView.h"

using namespace vrm;

DrawSceneRenderPass::DrawSceneRenderPass() : RenderPass() {}

DrawSceneRenderPass::~DrawSceneRenderPass() {}

void DrawSceneRenderPass::onInit() {}

void DrawSceneRenderPass::onSetup(const RenderPassContext& ctx) {}

void DrawSceneRenderPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("DrawSceneRenderPass::onRender");

  if (ctx.views.empty()) { return; }

  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");
  VRM_ASSERT_MSG(framebufferTarget->isCreated(), "Framebuffer is not created");

  framebufferTarget->bind();

  setupFaceCulling();

  if (framebufferTarget->isDepthAttachmentUsed()) { glEnable(GL_DEPTH_TEST); }
  else { glDisable(GL_DEPTH_TEST); }

  renderMeshes(ctx);
}

void DrawSceneRenderPass::setupFaceCulling() const
{
  if (faceCulling == EFaceCulling::eNone) { glDisable(GL_CULL_FACE); }
  else
  {
    glEnable(GL_CULL_FACE);
    GLenum mode;

    switch (faceCulling)
    {
    case EFaceCulling::eCullFront:
      mode = GL_FRONT;
      break;
      ;
    case EFaceCulling::eCullBack:
    default:
      mode = GL_BACK;
      break;
    }

    glCullFace(mode);

    switch (frontFace)
    {
    case EFrontFace::eCW:
      mode = GL_CW;
      break;
    case EFrontFace::eCCW:
    default:
      mode = GL_CCW;
      break;
    }

    glFrontFace(mode);
  }
}

void DrawSceneRenderPass::renderMeshes(const RenderPassContext& ctx) const
{
  for (const auto& [id, queuedMesh] : *meshRegistry)
  {
    if (!queuedMesh.material.isValid() || (queuedMesh.tags & meshTags).none())
    {
      continue;
    }

    size_t textOffset = queuedMesh.material->getTextures().size();

    const PassMaterial& passMat = getPassMaterial(queuedMesh.material);
    const auto&         shader  = passMat.getShader();
    shader.bind();
    shader.setUniformMat4f("u_Model", *queuedMesh.model);
    shader.setUniform1ui("u_EntityId", queuedMesh.entityId);

    if (shadowsEnable)
    {
      VRM_ASSERT(dirLightShadowMaps);
      shader.setTexture("u_DirectionalShadowMaps", *dirLightShadowMaps,
                        textOffset++);
      shader.setUniform1ui("u_SoftShadowKernelRadius",
                           ctx.dynamicSettings->shadows.softShadowKernelRadius);
    }

    if (bloomEnable)
    {
      shader.setUniform1f("u_bloomThreshold",
                          ctx.dynamicSettings->bloom.threshold);
    }

    applyStorageBufferParameters(shader);
    queuedMesh.material->applyUniforms(shader);

    const auto& mesh = *queuedMesh.mesh;

    gl::VertexArray::Bind(mesh.getVertexArray());
    gl::Buffer::Bind(mesh.getIndexBuffer(), GL_ELEMENT_ARRAY_BUFFER);

    for (size_t i = 0; i < ctx.views.size(); ++i)
    {
      const render::View&     view = ctx.views.at(i);
      const render::Viewport& vp   = view.getViewport();
      glViewport(vp.getOrigin().x, vp.getOrigin().y, vp.getSize().x,
                 vp.getSize().y);

      applyCameraUniforms(shader, *view.getCamera());
      applyViewportUniforms(shader, view.getViewport());
      if (clusteredLightsBuffer)
        shader.setStorageBuffer("ClusterInfoBlock", *clusteredLightsBuffer,
                                clusteredLightPerViewSize * i,
                                clusteredLightPerViewSize);

      glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(),
                     GL_UNSIGNED_INT, nullptr);
    }
  }
}

void DrawSceneRenderPass::applyStorageBufferParameters(
  const gl::Shader& shader) const
{
  for (const auto& param : storageBufferParameters)
  {
    if (param.size > 0)
      shader.setStorageBuffer(param.name, *param.buffer, param.offset,
                              param.size);
    else shader.setStorageBuffer(param.name, *param.buffer);
  }
}
