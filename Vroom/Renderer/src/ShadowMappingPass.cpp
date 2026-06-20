#include "Renderer/ShadowMappingPass.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "AssetManager/AssetManager.h"
#include "Core/Profiling.h"
#include "Rasterizer/AutoBuffer.h"
#include "Renderer/Aabb.h"
#include "Renderer/CameraBasic.h"
#include "Renderer/Frustum.h"
#include "Renderer/LightRegistry.h"
#include "Renderer/RawCamera.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderView.h"
#include "glm/ext/matrix_transform.hpp"

using namespace vrm;

namespace
{

/**
 * @brief Generates a mesh representing the NDC cube inverse-projected by the suppied viewProj matrix. Helps visualizing
 * the view-projection matrix of a camera.
 *
 * @param viewProj
 * @return MeshData
 */
MeshData GenerateViewVolumeMesh(const glm::mat4& viewProj)
{
  std::vector<Vertex>   vertices(6 * 4);
  std::vector<uint32_t> indices(6 * 2 * 3);

  Frustum   frustum     = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  glm::mat4 viewProjInv = glm::inverse(viewProj);

  frustum.transform(viewProjInv);

  auto corner = [&](int x, int y, int z)
  {
    return frustum.getCorner(x == 0 ? Frustum::eLeft : Frustum::eRight, y == 0 ? Frustum::eDown : Frustum::eUp,
                             z == 0 ? Frustum::eNear : Frustum::eFar);
  };

  int  v       = 0;
  auto addQuad = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, int faceIdx)
  {
    int base                    = faceIdx * 4;
    vertices[base + 0].position = a;
    vertices[base + 1].position = b;
    vertices[base + 2].position = c;
    vertices[base + 3].position = d;

    glm::vec3 normal          = glm::normalize(glm::cross(b - a, d - a));
    vertices[base + 0].normal = normal;
    vertices[base + 1].normal = normal;
    vertices[base + 2].normal = normal;
    vertices[base + 3].normal = normal;

    int i          = faceIdx * 6;
    indices[i + 0] = base + 0;
    indices[i + 1] = base + 1;
    indices[i + 2] = base + 2;
    indices[i + 3] = base + 0;
    indices[i + 4] = base + 2;
    indices[i + 5] = base + 3;
  };

  // Near face
  addQuad(corner(0, 0, 0), // left-down-near
          corner(1, 0, 0), // right-down-near
          corner(1, 1, 0), // right-up-near
          corner(0, 1, 0), // left-up-near
          0);

  // Far face
  addQuad(corner(1, 0, 1), // right-down-far
          corner(0, 0, 1), // left-down-far
          corner(0, 1, 1), // left-up-far
          corner(1, 1, 1), // right-up-far
          1);

  // Left face
  addQuad(corner(0, 0, 1), // left-down-far
          corner(0, 0, 0), // left-down-near
          corner(0, 1, 0), // left-up-near
          corner(0, 1, 1), // left-up-far
          2);

  // Right face
  addQuad(corner(1, 0, 0), // right-down-near
          corner(1, 0, 1), // right-down-far
          corner(1, 1, 1), // right-up-far
          corner(1, 1, 0), // right-up-near
          3);

  // Top face
  addQuad(corner(0, 1, 0), // left-up-near
          corner(1, 1, 0), // right-up-near
          corner(1, 1, 1), // right-up-far
          corner(0, 1, 1), // left-up-far
          4);

  // Bottom face
  addQuad(corner(0, 0, 1), // left-down-far
          corner(1, 0, 1), // right-down-far
          corner(1, 0, 0), // right-down-near
          corner(0, 0, 0), // left-down-near
          5);

  return MeshData{ std::move(vertices), std::move(indices) };
}

RawCamera ConstructViewProjFromDirLight(const render::View& view, const glm::vec3& lightDir)
{
  Frustum cameraFrustum = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  cameraFrustum.transform(glm::inverse(view.getCamera()->getViewProjection()));
  // cameraFrustum is now in world space.

  glm::vec3 cameraFrustumCenter = { 0.f, 0.f, 0.f };
  for (const glm::vec3& corner : cameraFrustum.getCorners())
  {
    cameraFrustumCenter += corner;
  }
  cameraFrustumCenter /= cameraFrustum.getCorners().size();

  glm::mat4 lightViewMatrix =
    glm::lookAt(cameraFrustumCenter + lightDir, cameraFrustumCenter, glm::vec3{ 0.f, 1.f, 0.f });

  cameraFrustum.transform(lightViewMatrix);
  // cameraFrustum is now in the light's view space.

  // aabb is the bounding box of the frustum in the light view space
  Aabb aabb(cameraFrustum.getCorners().begin(), cameraFrustum.getCorners().end());

  // TODO: is it enough ? We want to be sure to catch every objects between the dir light (located at infinity) and the
  // viewed fragments.
  float zfactor = 100.f;

  glm::mat4 lightProjMatrix = glm::ortho(aabb.getMin().x, aabb.getMax().x, aabb.getMin().y, aabb.getMax().y,
                                         aabb.getMin().z * zfactor, aabb.getMax().z);

  RawCamera camera;
  camera.setViewMatrix(lightViewMatrix);
  camera.setProjectionMatrix(lightProjMatrix);

  return camera;
}

} // namespace

ShadowMappingPass::ShadowMappingPass(const std::string& name) : RenderPass(name)
{}

ShadowMappingPass::~ShadowMappingPass()
{}

void ShadowMappingPass::onInit()
{
  VRM_ASSERT(lights != nullptr);
  VRM_ASSERT(lightMatricesBuffer != nullptr);
  addDefine("VRM_SHADOW_PASS");
}

void ShadowMappingPass::onSetup(const RenderPassContext& ctx)
{
  VRM_PROFILE_SCOPE("ShadowMappingPass::onSetup");

  bool dirShadowCastersChanged = updateShadowCasters();
  if (dirShadowCastersChanged)
  {
    resetDepthMapsAndFramebuffers();
  }

  if (m_dirLightShadowCasters.size() == 0)
  {
    return;
  }

  m_lightMatrices.startRegistering();
  m_dirLightCameras.clear();
  m_debugDirLights.clear();
  m_dirLightCameras.reserve(m_dirLightShadowCasters.size());
  m_debugDirLights.reserve(m_dirLightShadowCasters.size());

  for (size_t i = 0; i < m_dirLightShadowCasters.size(); ++i)
  {
    size_t      shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight       = lights->getDirectionalLights().at(shadowCasterId);
    m_dirLightCameras.emplace_back(ConstructViewProjFromDirLight(ctx.views.front(), dirLight.direction));
    // VRM_LOG_TRACE("Light direction: {}", glm::to_string(dirLight.direction));
    m_debugDirLights.emplace_back(GenerateViewVolumeMesh(m_dirLightCameras.back().getViewProjection()));

    m_lightMatrices.submit(i, m_dirLightCameras.back().getViewProjection());
  }

  m_lightMatrices.endRegistering();

  lightMatricesBuffer->ensureCapacity(sizeof(glm::vec4) + m_lightMatrices.getElementCount() * sizeof(glm::mat4));
  {
    std::span<uint8_t> map = lightMatricesBuffer->mapWriteOnly(true);

    glm::uint* lightCount = reinterpret_cast<glm::uint*>(map.data());
    *lightCount           = static_cast<glm::uint>(m_lightMatrices.getElementCount());

    glm::mat4* matrices = reinterpret_cast<glm::mat4*>(map.data() + sizeof(glm::vec4));
    std::memcpy(matrices, m_lightMatrices.getRawData(), m_lightMatrices.getElementCount() * sizeof(glm::mat4));

    lightMatricesBuffer->unmap();
  }
}

void ShadowMappingPass::onRender(const RenderPassContext& ctx) const
{
  VRM_PROFILE_SCOPE("ShadowMappingPass::onRender");

  if (m_dirLightShadowCasters.size() == 0)
  {
    return;
  }

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_FRONT);
  glEnable(GL_DEPTH_TEST);

  for (size_t i = 0; i < m_dirLightShadowCasters.size(); ++i)
  {
    size_t      shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight       = lights->getDirectionalLights().at(shadowCasterId);

    const auto& framebuffer = m_frameBuffers.at(i);
    framebuffer.bind();
    framebuffer.clearDepth();

    renderMeshes(m_dirLightCameras.at(i), render::Viewport({ 0u, 0u }, { resolution, resolution }));
  }

  if (ctx.dynamicSettings->shadows.debugDirLights)
  {
    VRM_ASSERT(ctx.frameBufferTarget != nullptr);
    ctx.frameBufferTarget->bind();
    glDisable(GL_CULL_FACE);

    for (const render::View& view : ctx.views)
      renderDirLightsFrustums(view);
  }
}

bool ShadowMappingPass::updateShadowCasters()
{
  size_t dirLightsCount = lights->getDirectionalLights().getElementCount();
  size_t shadowCasters  = 0;
  bool   changed        = false;

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
    desc.dimension      = 2;
    desc.width          = res;
    desc.height         = res;
    desc.depth          = shadowCasters;
    desc.internalFormat = GL_DEPTH_COMPONENT24;
    desc.format         = GL_DEPTH_COMPONENT;
    desc.layered        = true;
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

void ShadowMappingPass::renderMeshes(const CameraBasic& camera, const render::Viewport& viewport) const
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

    gl::VertexArray::Bind(mesh.getVertexArray());
    gl::Buffer::Bind(mesh.getIndexBuffer(), GL_ELEMENT_ARRAY_BUFFER);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}

void ShadowMappingPass::renderDirLightsFrustums(const render::View& view) const
{
  const render::Viewport& vp = view.getViewport();
  glViewport(vp.getOrigin().x, vp.getOrigin().y, vp.getSize().x, vp.getSize().y);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(2.f);

  for (const auto& mesh : m_debugDirLights)
  {
    auto material = AssetManager::Get().getAsset<MaterialAsset>("Resources/Engine/Material/FrustumViewerMaterial.json");
    const auto& shader = getPassMaterial(material).getShader();
    shader.bind();
    shader.setUniformMat4f("u_Model", glm::mat4(1.f));
    applyCameraUniforms(shader, *view.getCamera());
    applyViewportUniforms(shader, vp);
    material->applyUniforms(shader);

    gl::VertexArray::Bind(mesh.getVertexArray());
    gl::Buffer::Bind(mesh.getIndexBuffer(), GL_ELEMENT_ARRAY_BUFFER);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
