#pragma once

#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/AutoBuffer.h"
#include "Vroom/Render/ParticleEmitterRegistry.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "glm/fwd.hpp"
#include <vector>

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

  protected:

    void onInit() override;
    void onSetup(const RenderPassContext &ctx) override;
    void onRender(const RenderPassContext &ctx) const override;

  private:

    void _updateEmittersData();
    void _updateParticleStates();
    void _bindEmittersData(const gl::Shader& shader) const;
    void _bindSpawnData(const gl::Shader& shader) const;
    void _bindIndirectCommands(const gl::Shader& shader) const;
    void _bindParticleStates(const gl::Shader& shader) const;
    void _bindParticleInstanceData(const gl::Shader& shader) const;

    void _uploadIndirectCommandsData() const;
    void _uploadEmitterData() const;

  private:

    struct RawParticleEmitterSpecs
    {
      glm::vec4 color;

      glm::vec3 initialPosition;
      float lifeTime;

      glm::vec3 initialVelocity;
      float emitRate;
      
      glm::vec3 acceleration;
      float _pad0;

      glm::vec3 initialScale;
      float _pad1;

      glm::vec3 scaleOverTime;
      float _pad2;
    };

    struct RawDrawElementsIndirectCommand
    {
      glm::uint count;
      glm::uint instanceCount = 0;
      glm::uint firstIndex;
      int       baseVertex;
      glm::uint baseInstance;
    };

  private:

    constexpr static glm::uvec3 s_updaterGroupSize = { 64, 1, 1 };

    std::vector<RawDrawElementsIndirectCommand> m_indirectCommands;
    std::vector<PassMaterial const*> m_particleMaterials;
    std::vector<RawParticleEmitterSpecs> m_emittersData;

    render::AutoBuffer m_emittersDataBuffer;
    render::AutoBuffer m_spawnCountersBuffer;
    render::AutoBuffer m_indirectCommandsBuffer;
    render::AutoBuffer m_particleStatesBuffer;
    render::AutoBuffer m_instanceDataBuffer;

    size_t m_maxParticleCount = 0;
    const PassMaterial* m_updaterMaterial = nullptr;

  };

}