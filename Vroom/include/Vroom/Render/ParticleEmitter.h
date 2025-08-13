#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct ParticleEmitterSpecs
  {
    float lifetTime = 10.f;
    float emitRate = 1.f;
    glm::vec3 color = { 0.f, 0.f, 0.f };
    glm::vec3 initialPosition = { 0.f, 0.f, 0.f };
    glm::vec3 initialVelocity = { 0.f, 1.f, 0.f };
    glm::vec3 acceleration = { 0.f, 0.f, 0.f };
  };

  class ParticleEmitter
  {
  public:

    ParticleEmitter();
    ~ParticleEmitter();

    ParticleEmitter& operator=(const ParticleEmitter& other) = delete;
    ParticleEmitter(const ParticleEmitter& other) = delete;

    ParticleEmitter& operator=(ParticleEmitter&& other) = delete;
    ParticleEmitter(ParticleEmitter&& other) = delete;
    
    inline void setSpecs(const ParticleEmitterSpecs& specs) { m_specs = specs; m_specsDirty = true; }

    inline const ParticleEmitterSpecs& getSpecs() const { return m_specs; }
    inline ParticleEmitterSpecs& getSpecs() { m_specsDirty = true; return m_specs; }

  private:

    ParticleEmitterSpecs m_specs;
    bool m_specsDirty = true;
  };

}
