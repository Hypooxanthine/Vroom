#include "Vroom/Render/Passes/ShadowMappingPass.h"

using namespace vrm;

ShadowMappingPass::ShadowMappingPass()
{
}

ShadowMappingPass::~ShadowMappingPass()
{

}

void ShadowMappingPass::onInit()
{
  m_frameBuffer.create(dirLightMaps->getWidth(), dirLightMaps->getHeight(), 1);
}

void ShadowMappingPass::onRender() const
{
  
}

void ShadowMappingPass::renderMeshes(const CameraBasic& camera) const
{
  // VRM_DEBUG_ASSERT_MSG(camera && *camera, "No camera set for rendering");

  // glViewport(viewportOrigin->x, viewportOrigin->y, viewportSize->x, viewportSize->y);
  // const auto cameraPos = (*camera)->getPosition();

  // for (const auto &[id, queuedMesh] : *meshRegistry)
  // {
  //   const auto& shader = queuedMesh.material->getShader();
  //     shader.bind();
  //     shader.setUniformMat4f("u_Model", *queuedMesh.model);
  //     applyCameraUniforms(queuedMesh.material->getShader());
  //     applyStorageBufferParameters(queuedMesh.material->getShader());
    
  //   queuedMesh.material->applyUniforms();

  //   const auto& mesh = *queuedMesh.mesh;

  //   mesh.getVertexArray().bind();
  //   mesh.getIndexBuffer().bind();

  //   glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  // }

}
