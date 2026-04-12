#pragma once

#include "Rasterizer/FrameBuffer.h"
#include "RenderObjects/Mesh.h"
#include "Renderer/LightRegistry.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderViewport.h"
#include "Tools/LinearRegistry.h"

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

  OrthographicCamera constructViewProjFromDirLight(const glm::vec3& direction);

  void renderDirLightsFrustums(const render::View& view) const;

private:

  std::vector<gl::FrameBuffer>      m_frameBuffers;
  std::vector<size_t>               m_dirLightShadowCasters;
  LinearRegistry<glm::mat4, size_t> m_lightMatrices;
  std::vector<OrthographicCamera>   m_dirLightCameras;
  std::vector<render::Mesh>         m_debugDirLights;
};

} // namespace vrm
