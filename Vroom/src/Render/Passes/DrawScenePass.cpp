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

void DrawSceneRenderPass::onRender() const
{
  VRM_ASSERT_MSG(framebufferTarget != nullptr, "Invalid framebuffer");
  VRM_ASSERT_MSG(framebufferTarget->isCreated(), "Framebuffer is not created");

  framebufferTarget->bind();
  framebufferTarget->clearColors();
  framebufferTarget->clearDepth();

  setupFaceCulling();

  if (framebufferTarget->isDepthAttachmentUsed() || framebufferTarget->isRenderBufferAttached())
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  renderMeshes();
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

void DrawSceneRenderPass::renderMeshes() const
{
  VRM_DEBUG_ASSERT_MSG(camera && *camera, "No camera set for rendering");

  glViewport(viewportOrigin->x, viewportOrigin->y, viewportSize->x, viewportSize->y);
  const auto cameraPos = (*camera)->getPosition();

  for (const auto &[id, queuedMesh] : *meshRegistry)
  {
    const auto& shader = queuedMesh.material->getShader();
      shader.bind();
      shader.setUniformMat4f("u_Model", *queuedMesh.model);
      applyCameraUniforms(queuedMesh.material->getShader());
      applyStorageBufferParameters(queuedMesh.material->getShader());
    
    queuedMesh.material->applyUniforms();

    const auto& mesh = *queuedMesh.mesh;

    mesh.getVertexArray().bind();
    mesh.getIndexBuffer().bind();

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }

}

void DrawSceneRenderPass::applyCameraUniforms(const gl::Shader& shader) const
{
  shader.setUniform2ui("u_ViewportSize", viewportSize->x, viewportSize->y);
  shader.setUniformMat4f("u_View", (*camera)->getView());
  shader.setUniformMat4f("u_Projection", (*camera)->getProjection());
  shader.setUniformMat4f("u_ViewProjection", (*camera)->getViewProjection());
  shader.setUniform3f("u_ViewPosition", (*camera)->getPosition());
  shader.setUniform1f("u_Near", (*camera)->getNear());
  shader.setUniform1f("u_Far", (*camera)->getFar());
}

void DrawSceneRenderPass::applyStorageBufferParameters(const gl::Shader& shader) const
{
  for (const auto& [name, storageBuffer] : storageBufferParameters)
  {
    shader.setStorageBuffer(name, *storageBuffer);
  }
}
