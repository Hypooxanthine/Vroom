#pragma once

#include "Vroom/Api.h"

#include <glm/glm.hpp>

namespace vrm
{

  class DeltaTime;

  struct VRM_API ParticleEmitterSpecs
  {
    float lifeTime = 10.f;
    float emitRate = 1.f;

    glm::vec3 color = { 0.f, 0.f, 0.f };

    glm::vec3 initialPosition = { 0.f, 0.f, 0.f };
    glm::vec3 initialVelocity = { 0.f, 1.f, 0.f };
    glm::vec3 acceleration = { 0.f, 0.f, 0.f };

    glm::vec3 initialScale = { 1.f, 1.f, 1.f };
    glm::vec3 scaleOverTime = { 1.f, 1.f, 1.f };
  };

  class VRM_API ParticleEmitter
  {
  public:

    ParticleEmitter();
    ~ParticleEmitter();

    ParticleEmitter& operator=(const ParticleEmitter& other) = delete;
    ParticleEmitter(const ParticleEmitter& other) = delete;

    ParticleEmitter& operator=(ParticleEmitter&& other) = default;
    ParticleEmitter(ParticleEmitter&& other) = default;

    void update(const DeltaTime& dt);
    
    inline void setSpecs(const ParticleEmitterSpecs& specs) { m_specs = specs; }

    inline const ParticleEmitterSpecs& getSpecs() const { return m_specs; }
    inline ParticleEmitterSpecs& getSpecs() { return m_specs; }

    inline float getTimeSinceStart() const { return m_timeAlive; }
    inline unsigned int getNextParticleCountToSpawn() const { return m_nextParticlesToSpawn; }
    inline float getNextParticleToSpawnStartingLifetime() const { return m_nextParticleSpawnLifetime; }

  private:

    ParticleEmitterSpecs m_specs;
    float m_timeAlive = 0.f;
    float m_lastSpawnedParticleStamp = 0.f;

    float m_nextParticleSpawnLifetime = 0.f;
    unsigned int m_nextParticlesToSpawn = 0;
  };

}
