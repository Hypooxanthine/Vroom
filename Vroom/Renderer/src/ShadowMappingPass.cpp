#include "Renderer/ShadowMappingPass.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <span>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "AssetManager/AssetManager.h"
#include "Core/Profiling.h"
#include "Rasterizer/AutoBuffer.h"
#include "Rasterizer/SSBO430Layout.h"
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

/**
 * @brief World-space corners of a camera's view frustum, recovered by inverse-projecting the NDC cube. Indices 0-3 are
 * the near-plane corners and 4-7 the far-plane corners (corner k pairs with corner k+4 along the same view ray).
 */
std::array<glm::vec3, 8> ComputeFrustumCornersWorld(const render::View& view)
{
  Frustum frustum = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  frustum.transform(glm::inverse(view.getCamera()->getViewProjection()));
  // frustum is now in world space.

  std::array<glm::vec3, 8>      out;
  std::span<const glm::vec3, 8> corners = frustum.getCorners();
  std::copy(corners.begin(), corners.end(), out.begin());
  return out;
}

/**
 * @brief Corners of the sub-frustum covering the view-space linear depth range [splitNear, splitFar].
 *
 * World position is an affine function of linear view depth along each frustum edge, so the sub-frustum corners are an
 * exact lerp between the full frustum's near and far corners.
 */
std::array<glm::vec3, 8> ComputeCascadeCornersWorld(const std::array<glm::vec3, 8>& full, float camNear, float camFar,
                                                    float splitNear, float splitFar)
{
  const float tNear = (splitNear - camNear) / (camFar - camNear);
  const float tFar  = (splitFar - camNear) / (camFar - camNear);

  std::array<glm::vec3, 8> out;
  for (int k = 0; k < 4; ++k)
  {
    const glm::vec3& nearCorner = full[k];
    const glm::vec3& farCorner  = full[k + 4];
    out[k]                      = glm::mix(nearCorner, farCorner, tNear);
    out[k + 4]                  = glm::mix(nearCorner, farCorner, tFar);
  }
  return out;
}

/**
 * @brief View-space far depth of each cascade, using the practical CSM blend between uniform and logarithmic splits.
 *        splits.back() == far. lambda: 0 = uniform, 1 = logarithmic.
 */
std::vector<float> ComputeCascadeSplits(float nearPlane, float farPlane, uint32_t count, float lambda)
{
  std::vector<float> splits(count);
  for (uint32_t i = 1; i <= count; ++i)
  {
    const float p            = static_cast<float>(i) / static_cast<float>(count);
    const float logSplit     = nearPlane * std::pow(farPlane / nearPlane, p);
    const float uniformSplit = nearPlane + (farPlane - nearPlane) * p;
    splits[i - 1]            = lambda * logSplit + (1.f - lambda) * uniformSplit;
  }
  return splits;
}

/**
 * @brief Fits a tight, texel-snapped ortho light camera around the supplied world-space frustum corners.
 */
RawCamera ConstructViewProjFromCorners(const std::array<glm::vec3, 8>& corners, const glm::vec3& lightDir,
                                       float resolution)
{
  glm::vec3 frustumCenter = { 0.f, 0.f, 0.f };
  for (const glm::vec3& corner : corners)
  {
    frustumCenter += corner;
  }
  frustumCenter /= corners.size();

  float radius = 0.f;
  for (const glm::vec3& corner : corners)
  {
    radius = glm::max(radius, glm::length(corner - frustumCenter));
  }
  // Constant texel size.
  radius = std::ceil(radius * 16.f) / 16.f;

  // Guard the lookAt up-vector against a near-vertical light direction.
  glm::vec3 up = glm::abs(glm::normalize(lightDir).y) > 0.99f ? glm::vec3{ 0.f, 0.f, 1.f } : glm::vec3{ 0.f, 1.f, 0.f };
  glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter + lightDir, frustumCenter, up);

  // TODO: is it enough ? We want to be sure to catch every objects between the dir light (located at infinity) and the
  // viewed fragments.
  float zfactor = 100.f;

  glm::mat4 lightProjMatrix = glm::ortho(-radius, radius, -radius, radius, -radius * zfactor, radius);

  // Texel snapping: move the shadow box origin in whole-texel increments so that camera translation shifts the
  // shadow map by integer texels rather than by an arbitrary sub-texel amount. It only works because texel size is kept
  // constant.
  glm::mat4 shadowMatrix  = lightProjMatrix * lightViewMatrix;
  glm::vec4 shadowOrigin  = shadowMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
  shadowOrigin           *= resolution * 0.5f; // NDC -> texel units

  glm::vec4 rounded = glm::round(shadowOrigin);
  glm::vec4 offset  = (rounded - shadowOrigin) * (2.f / resolution); // back to NDC
  offset.z          = 0.f;
  offset.w          = 0.f;

  lightProjMatrix[3] += offset;

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

  const auto& shadowSettings = ctx.dynamicSettings->shadows;
  m_cascadeCount             = std::clamp<uint32_t>(shadowSettings.cascadeCount, 1u, kMaxShadowCascades);

  updateShadowCasters();

  const size_t casterCount = m_dirLightShadowCasters.size();

  // The depth array / framebuffers only depend on the resolution, cascade count and number of casters.
  if (resolution != m_builtResolution || m_cascadeCount != m_builtCascadeCount || casterCount != m_builtCasterCount)
  {
    resetDepthMapsAndFramebuffers();
  }

  if (casterCount == 0)
  {
    return;
  }

  const size_t totalCascades = casterCount * m_cascadeCount;

  m_lightMatrices.clear();
  m_dirLightCameras.clear();
  m_debugDirLights.clear();
  m_lightMatrices.reserve(totalCascades);
  m_dirLightCameras.reserve(totalCascades);
  m_debugDirLights.reserve(totalCascades);

  // Cascades are fit to the first view's frustum (the engine assumes a single main view for shadows).
  const render::View& view    = ctx.views.front();
  const float         camNear = view.getCamera()->getNear();
  const float         camFar  = view.getCamera()->getFar();
  const float         shadowFar =
    shadowSettings.shadowDistance > 0.f ? glm::min(camFar, camNear + shadowSettings.shadowDistance) : camFar;

  const std::array<glm::vec3, 8> fullCorners = ComputeFrustumCornersWorld(view);
  const std::vector<float>       splits =
    ComputeCascadeSplits(camNear, shadowFar, m_cascadeCount, shadowSettings.cascadeSplitLambda);

  for (size_t i = 0; i < casterCount; ++i)
  {
    size_t      shadowCasterId = m_dirLightShadowCasters.at(i);
    const auto& dirLight       = lights->getDirectionalLights().at(shadowCasterId);

    for (uint32_t c = 0; c < m_cascadeCount; ++c)
    {
      const float splitNear = (c == 0) ? camNear : splits.at(c - 1);
      const float splitFar  = splits.at(c);

      const std::array<glm::vec3, 8> cascadeCorners =
        ComputeCascadeCornersWorld(fullCorners, camNear, camFar, splitNear, splitFar);

      m_dirLightCameras.emplace_back(
        ConstructViewProjFromCorners(cascadeCorners, dirLight.direction, static_cast<float>(resolution)));
      m_debugDirLights.emplace_back(GenerateViewVolumeMesh(m_dirLightCameras.back().getViewProjection()));
      m_lightMatrices.emplace_back(m_dirLightCameras.back().getViewProjection());
    }
  }

  // std430 layout of LightMatricesBlock (ShadingModelBase_Shader.json):
  render::SSBO430Layout layout;
  const auto            aLightCount   = layout.push<glm::uint>();
  const auto            aCascadeCount = layout.push<glm::uint>();
  const auto            aSplits       = layout.push<glm::vec4>(); // u_cascadeSplits[0]
  for (uint32_t c = 1; c < kMaxShadowCascades; ++c)
    layout.push<glm::vec4>(); // remaining split slots (no array-push on the layout)
  const size_t matricesOffset = layout.getAlignedSize();

  lightMatricesBuffer->ensureCapacity(matricesOffset + m_lightMatrices.size() * sizeof(glm::mat4));
  {
    std::span<uint8_t> map = lightMatricesBuffer->mapWriteOnly(true);

    std::memset(map.data(), 0, matricesOffset); // Zero the header and any unused split slots.

    *reinterpret_cast<glm::uint*>(map.data() + aLightCount.getLocation())   = static_cast<glm::uint>(casterCount);
    *reinterpret_cast<glm::uint*>(map.data() + aCascadeCount.getLocation()) = static_cast<glm::uint>(m_cascadeCount);

    for (uint32_t c = 0; c < m_cascadeCount; ++c)
    {
      *reinterpret_cast<float*>(map.data() + aSplits.getLocation() + c * sizeof(glm::vec4)) = splits.at(c);
    }

    glm::mat4* matrices = reinterpret_cast<glm::mat4*>(map.data() + matricesOffset);
    std::memcpy(matrices, m_lightMatrices.data(), m_lightMatrices.size() * sizeof(glm::mat4));

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
    for (uint32_t c = 0; c < m_cascadeCount; ++c)
    {
      const size_t idx = i * m_cascadeCount + c;

      const auto& framebuffer = m_frameBuffers.at(idx);
      framebuffer.bind();
      framebuffer.clearDepth();

      renderMeshes(m_dirLightCameras.at(idx), render::Viewport({ 0u, 0u }, { resolution, resolution }));
    }
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

  // One depth-array layer / framebuffer per (light, cascade).
  const size_t layerCount = m_dirLightShadowCasters.size() * m_cascadeCount;

  // Record the signature the resources are now built for.
  m_builtResolution   = resolution;
  m_builtCascadeCount = m_cascadeCount;
  m_builtCasterCount  = m_dirLightShadowCasters.size();

  if (layerCount == 0)
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
    desc.depth          = layerCount;
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

  m_frameBuffers.resize(layerCount);

  for (size_t i = 0; i < layerCount; ++i)
  {
    auto& fb = m_frameBuffers.at(i);
    fb.create(res, res, 1);
    fb.setDepthAttachment(*depthTextureArray, static_cast<GLuint>(i), 0, 1.f);
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
