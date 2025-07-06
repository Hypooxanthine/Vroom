#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Helpers/StorageBufferRegistry.h"
#include "Vroom/Render/Clustering/LightRegistry.h"
#include "Vroom/Render/Camera/OrthographicCamera.h"

namespace vrm::render
{
  class AutoBuffer;
}

namespace vrm
{

  class MeshRegistry;
  class CameraBasic;
  class RenderViewport;
  class OrthographicCamera;
  class RenderMesh;

  class ShadowMappingPass : public RenderPass
  {
  public:
    ShadowMappingPass();
    virtual ~ShadowMappingPass();

    uint32_t resolution;
    LightRegistry* lights = nullptr;
    const MeshRegistry* meshRegistry = nullptr;
    gl::Texture* depthTextureArray = nullptr;
    render::AutoBuffer* lightMatricesBuffer = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;

    bool updateShadowCasters();

    void resetDepthMapsAndFramebuffers();

    void renderMeshes(const CameraBasic& camera, const RenderViewport& viewport) const;

    OrthographicCamera constructViewProjFromDirLight(const CameraBasic& renderCamera, const glm::vec3& direction);

    void renderDirLightsFrustums(const RenderPassContext& ctx) const;

  private:

    std::vector<gl::FrameBuffer> m_frameBuffers;
    std::vector<size_t> m_dirLightShadowCasters;
    LinearRegistry<glm::mat4, size_t> m_lightMatrices;
    std::vector<OrthographicCamera> m_dirLightCameras;
    std::vector<RenderMesh> m_debugDirLights;

  };

} // namespace vrm
