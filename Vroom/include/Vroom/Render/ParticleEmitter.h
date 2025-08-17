#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include <glm/glm.hpp>

namespace vrm
{

  class DeltaTime;

  class VRM_API ParticleEmitter
  {
  public:

    template <glm::length_t Dim>
    struct Attribute
    {
      using VectorType = glm::vec<Dim, float>;

      Attribute(const VectorType& singleValue) : spawnValue(singleValue), deathValue(singleValue) {}
      Attribute(const VectorType& spawn, const VectorType& death) : spawnValue(spawn), deathValue(death) {}
      Attribute() : Attribute(VectorType(0.f)) {}

      VectorType spawnValue;
      VectorType deathValue;
    };

    struct Specs
    {
      float lifeTime = 10.f;
      float emitRate = 1.f;

      Attribute<4> color = { { 0.f, 0.f, 0.f, 1.f } };
      Attribute<3> position = { { 0.f, 0.f, 0.f } };
      Attribute<3> scale = { { 1.f, 1.f, 1.f } };
    };

  public:

    ParticleEmitter();
    ~ParticleEmitter();

    ParticleEmitter(MeshAsset::Handle meshAsset, MaterialAsset::Handle materialAsset);

    ParticleEmitter& operator=(const ParticleEmitter& other) = delete;
    ParticleEmitter(const ParticleEmitter& other) = delete;

    ParticleEmitter& operator=(ParticleEmitter&& other) = default;
    ParticleEmitter(ParticleEmitter&& other) = default;

    void update(const DeltaTime& dt);

    inline bool isDirty() const { return m_dirty; }
    inline void undirtify() const { m_dirty = false; }
    
    inline void setSpecs(const Specs& specs) { m_specs = specs; m_dirty = true; }
    inline void setMesh(MeshAsset::Handle meshAsset) { m_mesh = meshAsset; }
    inline void setMaterial(MaterialAsset::Handle materialAsset) { m_material = materialAsset; }

    inline const Specs& getSpecs() const { return m_specs; }
    inline Specs& getSpecs() { m_dirty = true; return m_specs; }
    inline MeshAsset::Handle getMesh() const { return m_mesh; }
    inline MaterialAsset::Handle getMaterial() const { return m_material; }

    inline float getTimeSinceStart() const { return m_timeAlive; }
    inline unsigned int getNextParticleCountToSpawn() const { return m_nextParticlesToSpawn; }
    inline float getNextParticleToSpawnStartingLifetime() const { return m_nextParticleSpawnLifetime; }

  private:

    Specs m_specs;
    mutable bool m_dirty = true;
    MeshAsset::Handle m_mesh;
    MaterialAsset::Handle m_material;
    
    float m_timeAlive = 0.f;
    float m_lastSpawnedParticleStamp = 0.f;

    float m_nextParticleSpawnLifetime = 0.f;
    unsigned int m_nextParticlesToSpawn = 0;
  };

}
