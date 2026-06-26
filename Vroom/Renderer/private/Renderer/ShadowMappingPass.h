#pragma once

#include "Rasterizer/FrameBuffer.h"
#include "RenderObjects/Mesh.h"
#include "Renderer/LightRegistry.h"
#include "Renderer/MeshRegistry.h"
#include "Renderer/RawCamera.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderViewport.h"


namespace vrm::render
{
class AutoBuffer;
class View;
} // namespace vrm::render

namespace vrm
{

class MeshRegistry;
class CameraBasic;
class OrthographicCamera;

class ShadowMappingPass : public RenderPass
{
public:

  ShadowMappingPass(const std::string& name = "ShadowMappingPass");
  virtual ~ShadowMappingPass();

  // Maximum number of cascades per directional light. Must stay in sync with
  // VRM_MAX_SHADOW_CASCADES in Resources/Engine/Shader/CommonLights.glsl, which
  // sizes the u_cascadeSplits array of the LightMatricesBlock SSBO.
  static constexpr uint32_t kMaxShadowCascades = 5;

  uint32_t            resolution;
  LightRegistry*      lights              = nullptr;
  const MeshRegistry* meshRegistry        = nullptr;
  gl::Texture*        depthTextureArray   = nullptr;
  render::AutoBuffer* lightMatricesBuffer = nullptr;

protected:

  virtual void onInit() override;

  virtual void onSetup(const RenderPassContext& ctx) override;

  virtual void onRender(const RenderPassContext& ctx) const override;

  bool updateShadowCasters();

  void resetDepthMapsAndFramebuffers();

  void renderMeshes(const CameraBasic& camera, const render::Viewport& viewport) const;

  void renderDirLightsFrustums(const render::View& view) const;

private:

  // Cascades are flat-indexed as (lightIndex * m_cascadeCount + cascade) across
  // the framebuffers, cameras, depth-array layers and matrices.
  std::vector<gl::FrameBuffer> m_frameBuffers;
  std::vector<size_t>          m_dirLightShadowCasters;
  std::vector<glm::mat4>       m_lightMatrices;
  std::vector<RawCamera>       m_dirLightCameras;
  std::vector<render::Mesh>    m_debugDirLights;

  uint32_t m_cascadeCount = 0;

  // Signature of the currently allocated depth array / framebuffers. When any of
  // these change, the GPU resources are rebuilt.
  uint32_t m_builtResolution   = 0;
  uint32_t m_builtCascadeCount = 0;
  size_t   m_builtCasterCount  = 0;
};

} // namespace vrm
