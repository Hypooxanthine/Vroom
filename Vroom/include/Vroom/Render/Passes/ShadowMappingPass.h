#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Abstraction/ArrayTexture2D.h"

namespace vrm::gl
{
  
}

namespace vrm
{

  class MeshRegistry;
  class LightRegistry;
  class CameraBasic;
  class RenderViewport;
  class OrthographicCamera;

  class ShadowMappingPass : public RenderPass
  {
  public:
    ShadowMappingPass();
    virtual ~ShadowMappingPass();

    uint32_t resolution;
    LightRegistry* lights = nullptr;
    const MeshRegistry* meshRegistry = nullptr;
    gl::ArrayTexture2D* depthTextures = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;

    bool updateShadowCasters();

    void resetDepthMapsAndFramebuffers();

    void renderMeshes(const CameraBasic& camera, const RenderViewport& viewport) const;

    static OrthographicCamera constructViewProjFromDirLight(const CameraBasic& renderCamera, const glm::vec3& direction);

  private:

    std::vector<gl::FrameBuffer> m_frameBuffers;
    std::vector<size_t> m_dirLightShadowCasters;

  };

} // namespace vrm
