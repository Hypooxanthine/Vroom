#pragma once

#include <glm/glm.hpp>

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/AutoBuffer.h"

namespace vrm::gl
{
  class FrameBuffer;
  class Buffer;
}

namespace vrm
{

  class CameraBasic;
  class LightRegistry;
  
  class LightClusteringPass : public RenderPass
  {
  public:
    
    LightClusteringPass();
    virtual ~LightClusteringPass();

    const gl::Buffer& getClustersBuffer() const { return m_clustersBuffer.getBuffer(); }
    size_t getPerViewSize() const;

  public:
  
    glm::uvec3 clusterCount = { 1, 1, 1 };
    const gl::Buffer* lightsStorageBuffer = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;

  private:

    static constexpr glm::uvec3 s_setupClustersLocalSize = { 4, 4, 8 };
    size_t m_lastViewCount = 0;

    const PassMaterial* m_setupClustersMat = nullptr;
    const PassMaterial* m_cullLightsMat = nullptr;

    render::AutoBuffer m_clustersBuffer;
    unsigned int m_totalClusters = 0;
  };

} // namespace vrm
