#pragma once

#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/AutoBuffer.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/ParticleEmitterRegistry.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "glm/fwd.hpp"

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

  public:

    const ParticleEmitterRegistry* emitters = nullptr;

  private:

    struct EmitterData
    {
      glm::uint particleOffset;
      ParticleEmitterSpecs specs;
    };

  protected:

    void onInit() override;
    void onSetup(const RenderPassContext &ctx) override;
    void onRender(const RenderPassContext &ctx) const override;

  private:

    void _updateEmittersData();
    void _updateParticleStates();
    void _bindEmittersData(const gl::Shader& shader) const;
    void _bindParticleStates(const gl::Shader& shader) const;

  private:

    constexpr static glm::uvec3 s_updaterGroupSize = { 64, 1, 1 };

    render::AutoBuffer m_emittersDataBuffer;
    render::AutoBuffer m_particleStatesBuffer;

    size_t m_maxParticleCount = 0;
    const PassMaterial* m_updaterMaterial = nullptr;

  };

}