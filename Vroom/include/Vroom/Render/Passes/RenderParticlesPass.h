#pragma once

#include "Vroom/Render/AutoBuffer.h"
#include "Vroom/Render/Passes/RenderPass.h"
namespace vrm
{

  class RenderParticlesPass : public RenderPass
  {
  public:

    RenderParticlesPass();
    ~RenderParticlesPass();

    RenderParticlesPass& operator=(const RenderParticlesPass& other) = delete;
    RenderParticlesPass(const RenderParticlesPass& other) = delete;

    RenderParticlesPass& operator=(RenderParticlesPass&& other) = delete;
    RenderParticlesPass(RenderParticlesPass&& other) = delete;

  protected:

    void onInit() override;
    void onSetup(const RenderPassContext &ctx) override;
    void onRender(const RenderPassContext &ctx) const override;

  private:

    render::AutoBuffer m_particleStatesBuffer;

  };

}