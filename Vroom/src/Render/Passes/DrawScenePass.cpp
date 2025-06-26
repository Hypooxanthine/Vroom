#include "Vroom/Render/Passes/DrawScenePass.h"

#include "Vroom/Core/Assert.h"

#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Abstraction/StorageBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

using namespace vrm;

DrawSceneRenderPass::DrawSceneRenderPass()
  : RenderPass()
{

}

DrawSceneRenderPass::~DrawSceneRenderPass()
{

}

void DrawSceneRenderPass::onRender(const RenderPassContext& ctx) const
{
  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");
  VRM_ASSERT_MSG(framebufferTarget->isCreated(), "Framebuffer is not created");
  VRM_ASSERT(ctx.mainCamera != nullptr);

  framebufferTarget->bind();

  setupFaceCulling();

  if (framebufferTarget->isDepthAttachmentUsed())
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  renderMeshes(ctx);
}

void DrawSceneRenderPass::setupFaceCulling() const
{
  if (faceCulling == EFaceCulling::eNone)
  {
    glDisable(GL_CULL_FACE);
  }
  else
  {
    glEnable(GL_CULL_FACE);
    GLenum mode;

    switch(faceCulling)
    {
    case EFaceCulling::eCullFront:
      mode = GL_FRONT;
      break;;
    case EFaceCulling::eCullBack:
    default:
      mode = GL_BACK;
      break;
    }

    glCullFace(mode);

    switch(frontFace)
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
  VRM_ASSERT(entityPickingTex != 0);
  glViewport(ctx.viewport.getOrigin().x, ctx.viewport.getOrigin().y, ctx.viewport.getSize().x, ctx.viewport.getSize().y);

  for (const auto &[id, queuedMesh] : *meshRegistry)
  {
    if (!queuedMesh.visible || !queuedMesh.material.isValid())
    {
      continue;
    }

    size_t textOffset = queuedMesh.material->getTextures().size(); 

    const auto& shader = queuedMesh.material->getShader(); 
      shader.bind();
      shader.setUniformMat4f("u_Model", *queuedMesh.model);
      shader.setUniform1ui("u_EntityId", queuedMesh.entityId);
      
      if (shadowsEnable)
      {
        VRM_ASSERT(dirLightShadowMaps);
        shader.setTexture("u_DirectionalShadowMaps", *dirLightShadowMaps, textOffset++);
        shader.setUniform1ui("u_SoftShadowKernelRadius", softShadowKernelRadius);
      }
      
      int imageTextureUnit = textOffset++; 
      GLCall(glBindImageTexture(imageTextureUnit, entityPickingTex, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_R32UI));
      shader.setUniform1i("u_entityPickingImage", imageTextureUnit);

      applyCameraUniforms(shader, *ctx.mainCamera);
      applyViewportUniforms(shader, ctx.viewport);
      applyStorageBufferParameters(shader);
    
    queuedMesh.material->applyUniforms();

    const auto& mesh = *queuedMesh.mesh;

    mesh.getVertexArray().bind();
    mesh.getIndexBuffer().bind();

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }

}

void DrawSceneRenderPass::applyStorageBufferParameters(const gl::Shader& shader) const
{
  for (const auto& [name, storageBuffer] : storageBufferParameters)
  {
    shader.setStorageBuffer(name, *storageBuffer);
  }
}
