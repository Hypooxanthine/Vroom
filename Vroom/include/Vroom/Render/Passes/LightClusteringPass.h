#pragma once

#include <glm/glm.hpp>

#include "Vroom/Render/Passes/RenderPass.h"

namespace vrm::gl
{
  class FrameBuffer;
  class Buffer;
}

namespace vrm
{

  class CameraBasic;
  class LightRegistry;
  class ClusteredLights;
  
  class LightClusteringPass : public RenderPass
  {
  public:
    
    LightClusteringPass();
    virtual ~LightClusteringPass();

  public:
  
    glm::uvec3 clusterCount = { 1, 1, 1 };
    const gl::Buffer* lightsStorageBuffer = nullptr;
    ClusteredLights* clusteredLights = nullptr;

  protected:

    virtual void onRender(const RenderPassContext& ctx) const override;
  };

} // namespace vrm
