#include "Vroom/Render/Passes/ShadowMappingPass.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Vroom/Asset/AssetManager.h"

#include "Vroom/Render/Aabb.h"
#include "Vroom/Render/Frustum.h"

#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Clustering/LightRegistry.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

using namespace vrm;

ShadowMappingPass::ShadowMappingPass()
{
  addDefine("VRM_SHADOW_PASS");
}

ShadowMappingPass::~ShadowMappingPass()
{

}

void ShadowMappingPass::onInit()
{
  VRM_ASSERT(lights != nullptr);
  VRM_ASSERT(lightMatricesStorageBuffer != nullptr);
  lightMatricesStorageBuffer->reset(sizeof(glm::vec4) + sizeof(glm::mat4) * LightRegistry::s_maxDirLights);
  m_lightMatricesSBR = std::make_unique<decltype(m_lightMatricesSBR)::element_type>(lightMatricesStorageBuffer, 0);
}

void ShadowMappingPass::onSetup(const RenderPassContext& ctx)
{
  bool dirShadowCastersChanged = updateShadowCasters();
  if (dirShadowCastersChanged)
  {
    resetDepthMapsAndFramebuffers();
  }

  if (m_dirLightShadowCasters.size() == 0)
  {
    return;
  }

  m_lightMatricesSBR->startRegistering();
  m_dirLightCameras.clear();
  m_debugDirLights.clear();
  m_dirLightCameras.reserve(m_dirLightShadowCasters.size());
  m_debugDirLights.reserve(m_dirLightShadowCasters.size());

  for (size_t i = 0; i < m_dirLightShadowCasters.size(); ++i)
  {
    size_t shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight = lights->getDirectionalLights().at(shadowCasterId);
    m_dirLightCameras.emplace_back(constructViewProjFromDirLight(*ctx.mainCamera, dirLight.direction));
    // VRM_LOG_TRACE("Light direction: {}", glm::to_string(dirLight.direction));
    m_debugDirLights.emplace_back(m_dirLightCameras.back().generateViewVolumeMesh());

    m_lightMatricesSBR->submit(i, m_dirLightCameras.back().getViewProjection());
  }

  m_lightMatricesSBR->endRegistering();

}

void ShadowMappingPass::onRender(const RenderPassContext& ctx) const
{
  if (m_dirLightShadowCasters.size() == 0)
  {
    return;
  }
  
  VRM_ASSERT(ctx.mainCamera != nullptr);

  bool debugDirLights = false;

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_FRONT);
  glEnable(GL_DEPTH_TEST);

  for (size_t i = 0; i < m_dirLightShadowCasters.size(); ++i)
  {
    size_t shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight = lights->getDirectionalLights().at(shadowCasterId);

    const auto& framebuffer = m_frameBuffers.at(i);
    framebuffer.bind();
    framebuffer.clearDepth();

    renderMeshes(m_dirLightCameras.at(i), RenderViewport({ 0u, 0u }, { resolution, resolution }));
  }

  if (debugDirLights)
  {
    VRM_ASSERT(ctx.frameBufferTarget != nullptr);
    ctx.frameBufferTarget->bind();
    glDisable(GL_CULL_FACE);

    renderDirLightsFrustums(ctx);
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
  VRM_ASSERT(depthTextureArray != nullptr);
  size_t shadowCasters = m_dirLightShadowCasters.size();
  
  if (shadowCasters == 0)
  {
    depthTextureArray->release();
    m_frameBuffers.clear();
    return;
  }

  auto res = static_cast<GLsizei>(resolution);
  
  gl::Texture::Desc desc;
  {
    desc.dimension = 2;
    desc.width = res;
    desc.height = res;
    desc.depth = shadowCasters;
    desc.internalFormat = GL_DEPTH_COMPONENT24;
    desc.format = GL_DEPTH_COMPONENT;
    desc.layered = true;
  }
  depthTextureArray->create(desc);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  static constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

  m_frameBuffers.resize(shadowCasters);

  // static gl::ArrayTexture2D debugColors;
  // debugColors.createColor(res, res, 4, static_cast<GLsizei>(shadowCasters));

  for (size_t i = 0; i < shadowCasters; ++i)
  {
    auto& fb = m_frameBuffers.at(i);
    fb.create(res, res, 1);
    fb.setDepthAttachment(*depthTextureArray, static_cast<GLuint>(i), 0, 1.f);
    // fb.setColorAttachment(0, debugColors, static_cast<GLuint>(i));
    fb.validate();
  }
}

void ShadowMappingPass::renderMeshes(const CameraBasic& camera, const RenderViewport& viewport) const
{
  glViewport(viewport.getOrigin().x, viewport.getOrigin().y, viewport.getSize().x, viewport.getSize().y);

  for (const auto& [id, queuedMesh] : *meshRegistry)
  {
    if (!queuedMesh.material.isValid() || !queuedMesh.tags.test(EMeshTag::eShadowCaster))
    {
      continue;
    }
    
    const auto& shader = getPassMaterial(queuedMesh.material).getShader();
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

OrthographicCamera ShadowMappingPass::constructViewProjFromDirLight(const CameraBasic& renderCamera, const glm::vec3& direction)
{
  // {
  //   const float width = 10.f, height = 10.f, depth = 100.f;
  //   const float near = 0.1f;
  //   const float far = near + depth;

  //   OrthographicCamera out(width, height, near, far);
  //   out.setWorldPosition(direction * depth / 2.f);
  //   out.setViewDir(-direction);

  //   return out;
  // }

  /* Simple algo : light frustum looking towards view position */

  // if (false)
  {
    glm::vec3 viewPos = renderCamera.getPosition();

    float width = 50.f, height = width, depth = 100.f;
    float near = 0.1f;
    float far = depth - near;

    OrthographicCamera out(width, height, near, far);
    out.setWorldPosition(viewPos + direction * depth / 2.f);
    out.setViewDir(-direction);

    return out;
  }

  /* Trying the algorithm "view frustum always inside light frustum". Couldnt make it work for now :) */

  const glm::mat4& viewProjInv = glm::inverse(renderCamera.getViewProjection());

  Frustum frustum_world = Frustum::CreateFromAabb(Aabb::GetNDC(), true);

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
    glm::mat4 world_to_light = glm::lookAt(lightPos_world_space, lightPos_world_space + direction, up); // Now we know the real light position

    Frustum frustum_light_space = frustum_world;
    frustum_light_space.transform(world_to_light);

    finalAabb = Aabb::CreateFromFrustum(frustum_light_space);
  }

  const float width = finalAabb.calcWidth(), height = finalAabb.calcHeight(), depth = finalAabb.calcDepth();
  const float near = 0.1f;
  const float far = near + depth;

  OrthographicCamera out(width, height, near, far);
  out.setWorldPosition(lightPos_world_space);
  out.setViewDir(-direction);

  return out;
}

void ShadowMappingPass::renderDirLightsFrustums(const RenderPassContext& ctx) const
{
  glViewport(ctx.viewport.getOrigin().x, ctx.viewport.getOrigin().y, ctx.viewport.getSize().x, ctx.viewport.getSize().y);
  
  for (const auto& mesh : m_debugDirLights)
  {
    auto material = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/FrustumViewerMaterial.json");
    const auto& shader = getPassMaterial(material, nullptr).getShader();
    shader.bind();
    shader.setUniformMat4f("u_Model", glm::mat4(1.f));
    applyCameraUniforms(shader, *ctx.mainCamera);
    applyViewportUniforms(shader, ctx.viewport);
    material->applyUniforms(shader);


    mesh.getVertexArray().bind();
    mesh.getIndexBuffer().bind();

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}
