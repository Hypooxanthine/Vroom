#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <glm/glm.hpp>

namespace vrm
{

  class DeltaTime;

  struct VRM_API ParticleEmitterSpecs
  {
    float lifeTime = 10.f;
    float emitRate = 1.f;

    glm::vec4 color = { 0.f, 0.f, 0.f, 1.f };

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

    ParticleEmitter(MeshAsset::Handle meshAsset, MaterialAsset::Handle materialAsset);

    ParticleEmitter& operator=(const ParticleEmitter& other) = delete;
    ParticleEmitter(const ParticleEmitter& other) = delete;

    ParticleEmitter& operator=(ParticleEmitter&& other) = default;
    ParticleEmitter(ParticleEmitter&& other) = default;

    void update(const DeltaTime& dt);
    
    inline void setSpecs(const ParticleEmitterSpecs& specs) { m_specs = specs; }
    inline void setMesh(MeshAsset::Handle meshAsset) { m_mesh = meshAsset; }
    inline void setMaterial(MaterialAsset::Handle materialAsset) { m_material = materialAsset; }

    inline const ParticleEmitterSpecs& getSpecs() const { return m_specs; }
    inline MeshAsset::Handle getMesh() const { return m_mesh; }
    inline MaterialAsset::Handle getMaterial() const { return m_material; }

    inline float getTimeSinceStart() const { return m_timeAlive; }
    inline unsigned int getNextParticleCountToSpawn() const { return m_nextParticlesToSpawn; }
    inline float getNextParticleToSpawnStartingLifetime() const { return m_nextParticleSpawnLifetime; }

  private:

    ParticleEmitterSpecs m_specs;
    MeshAsset::Handle m_mesh;
    MaterialAsset::Handle m_material;
    
    float m_timeAlive = 0.f;
    float m_lastSpawnedParticleStamp = 0.f;

    float m_nextParticleSpawnLifetime = 0.f;
    unsigned int m_nextParticlesToSpawn = 0;
  };

}
