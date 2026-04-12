#pragma once

#include "Renderer/ParticleEmitterRegistry.h"
#include "Renderer/RenderPass.h"

namespace vrm
{

  class RenderParticlesPass : public RenderPass
  {
  public:

    RenderParticlesPass(const std::string& name = "RenderParticlesPass");
    ~RenderParticlesPass();

    RenderParticlesPass& operator=(const RenderParticlesPass& other) = delete;
    RenderParticlesPass(const RenderParticlesPass& other) = delete;

    RenderParticlesPass& operator=(RenderParticlesPass&& other) = delete;
    RenderParticlesPass(RenderParticlesPass&& other) = delete;

  public:

    ParticleEmitterRegistry* emitters = nullptr;

  protected:

    void onInit() override;
    void onSetup(const RenderPassContext &ctx) override;
    void onRender(const RenderPassContext &ctx) const override;

  private:

  };

}