#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Abstraction/ArrayTexture2D.h"
#include "Vroom/Render/Helpers/StorageBufferRegistry.h"
#include "Vroom/Render/Clustering/LightRegistry.h"
#include "Vroom/Render/Camera/OrthographicCamera.h"

namespace vrm::gl
{
  class AutoResizeStorageBuffer;
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
    gl::ArrayTexture2D* depthTextures = nullptr;
    gl::AutoResizeStorageBuffer* lightMatricesStorageBuffer = nullptr;

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
    std::unique_ptr<StorageBufferRegistry<glm::mat4, size_t, LightRegistry::s_maxDirLights, sizeof(glm::vec4)>> m_lightMatricesSBR;
    std::vector<OrthographicCamera> m_dirLightCameras;
    std::vector<RenderMesh> m_debugDirLights;

  };

} // namespace vrm
