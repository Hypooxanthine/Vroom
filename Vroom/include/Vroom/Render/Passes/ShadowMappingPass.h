#pragma once

#include "Vroom/Render/Passes/RenderPass.h"

#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/MeshRegistry.h"

namespace vrm::gl
{
  class ArrayTexture2D;
}

class LightRegistry;
class CameraBasic;

namespace vrm
{

  class ShadowMappingPass : public RenderPass
  {
  public:
    ShadowMappingPass();
    virtual ~ShadowMappingPass();

    gl::ArrayTexture2D* dirLightMaps = nullptr;
    const LightRegistry* lights = nullptr;
    const MeshRegistry* meshRegistry = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onRender() const override;

    void renderMeshes(const CameraBasic& camera) const;

  private:

    gl::FrameBuffer m_frameBuffer;

  };

} // namespace vrm
