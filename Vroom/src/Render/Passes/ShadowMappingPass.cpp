#include "Vroom/Render/Passes/ShadowMappingPass.h"

#include <glm/gtx/rotate_vector.hpp>

#include "Vroom/Math/Aabb.h"
#include "Vroom/Math/Frustum.h"

#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Camera/OrthographicCamera.h"
#include "Vroom/Render/Clustering/LightRegistry.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

using namespace vrm;

ShadowMappingPass::ShadowMappingPass()
{
}

ShadowMappingPass::~ShadowMappingPass()
{

}

void ShadowMappingPass::onInit()
{
  VRM_ASSERT(lights != nullptr);
}

void ShadowMappingPass::onSetup(const RenderPassContext& ctx)
{
  bool dirShadowCastersChanged = updateShadowCasters();

  if (dirShadowCastersChanged)
  {
    resetDepthMapsAndFramebuffers();
  }
}

void ShadowMappingPass::onRender(const RenderPassContext& ctx) const
{
  VRM_ASSERT(ctx.mainCamera != nullptr);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_FRONT);
  glEnable(GL_DEPTH_TEST);

  for (size_t i = 0; i < m_dirLightShadowCasters.size(); ++i)
  {
    size_t shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight = lights->getDirectionalLights().at(shadowCasterId);

    OrthographicCamera camera = constructViewProjFromDirLight(*ctx.mainCamera, dirLight.direction);

    const auto& framebuffer = m_frameBuffers.at(i);
    framebuffer.bind();
    framebuffer.clearDepth();

    renderMeshes(camera, RenderViewport({ 0u, 0u }, { resolution, resolution }));
  }
}

bool ShadowMappingPass::updateShadowCasters()
{
  size_t dirLightsCount = lights->getDirectionalLights().getElementCount();
  size_t shadowCasters = 0;
  bool changed = false;

  for (size_t i = 0; i < dirLightsCount; ++i)
  {
    const auto& dirLight = lights->getDirectionalLights().at(i);
    if (dirLight.castsShadows)
    {
      if (shadowCasters >= m_dirLightShadowCasters.size() || m_dirLightShadowCasters.at(shadowCasters) != i)
      {
        changed = true;
      }

      ++shadowCasters;
    }
  }

  if (shadowCasters != m_dirLightShadowCasters.size())
  {
    changed = true;
  }

  if (!changed)
    return false;

  m_dirLightShadowCasters.resize(shadowCasters);
  shadowCasters = 0;

  for (size_t i = 0; i < dirLightsCount; ++i)
  {
    const auto& dirLight = lights->getDirectionalLights().at(i);
    if (dirLight.castsShadows)
    {
      m_dirLightShadowCasters.at(shadowCasters) = i;
      ++shadowCasters;
    }
  }

  return true;
}

void ShadowMappingPass::resetDepthMapsAndFramebuffers()
{
  VRM_ASSERT(depthTextures != nullptr);
  size_t shadowCasters = m_dirLightShadowCasters.size();

  auto res = static_cast<GLsizei>(resolution);
  depthTextures->createDepth(res, res, static_cast<GLsizei>(shadowCasters));
  m_frameBuffers.resize(shadowCasters);

  for (size_t i = 0; i < shadowCasters; ++i)
  {
    auto& fb = m_frameBuffers.at(i);
    fb.create(res, res, 1);
    fb.setDepthAttachment(*depthTextures, static_cast<GLuint>(i), 0, 1.f);
    fb.validate();
  }
}

void ShadowMappingPass::renderMeshes(const CameraBasic& camera, const RenderViewport& viewport) const
{
  glViewport(viewport.getOrigin().x, viewport.getOrigin().y, viewport.getSize().x, viewport.getSize().y);
  const auto cameraPos = camera.getPosition();

  for (const auto& [id, queuedMesh] : *meshRegistry)
  {
    const auto& shader = queuedMesh.material->getShadowCastingShader();
    shader.bind();
    shader.setUniformMat4f("u_Model", *queuedMesh.model);
    applyCameraUniforms(shader, camera);
    applyViewportUniforms(shader, viewport);

    const auto& mesh = *queuedMesh.mesh;

    mesh.getVertexArray().bind();
    mesh.getIndexBuffer().bind();

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }

}

OrthographicCamera ShadowMappingPass::constructViewProjFromDirLight(const CameraBasic& camera, const glm::vec3& direction)
{
  const glm::mat4& viewProjInv = glm::inverse(camera.getViewProjection());

  Frustum frustum_world = Frustum::CreateFromAabb(Aabb::GetNDC());

  frustum_world.transform(viewProjInv);

  glm::vec3 origin = { 0.f, 0.f, 0.f };
  glm::vec3 up = { 0.f, 1.f, 0.f };

  glm::vec3 lightPos_world_space; // Computing this position.
  {
    glm::mat4 world_to_light = glm::lookAt(origin, direction, up); // Origin is taken instead of real position
    glm::mat4 light_to_world = glm::inverse(world_to_light);

    Frustum frustum_light_space = frustum_world;
    frustum_light_space.transform(world_to_light);

    Aabb light_aabb = Aabb::CreateFromFrustum(frustum_light_space);
    glm::vec3 light_aabb_center = light_aabb.calcCenter();

    glm::vec3 lightPos_light_space = { light_aabb_center.x, light_aabb_center.y, light_aabb.getMin().z };
    lightPos_world_space = light_to_world * glm::vec4(lightPos_light_space, 1.f);
  }

  Aabb finalAabb;
  // Computing the real bounding box with the real origin
  {
    glm::mat4 world_to_light = glm::lookAt(lightPos_world_space, direction, up); // Now we know the real light position

    Frustum frustum_light_space = frustum_world;
    frustum_light_space.transform(world_to_light);

    finalAabb = Aabb::CreateFromFrustum(frustum_light_space);
  }

  const float width = finalAabb.calcWidth(), height = finalAabb.calcHeight(), depth = finalAabb.calcDepth();
  const float near = 0.01f;
  const float far = near + depth;

  OrthographicCamera out(width, height, near, far);
  out.setViewDir(direction);

  return out;
}
