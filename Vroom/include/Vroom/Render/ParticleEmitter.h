#pragma once

#include <cstddef>
#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "Vroom/Render/ParticleEmitterRender.h"

namespace vrm
{

class DeltaTime;
class ParticleEmitterRender;
class RenderPassContext;

class VRM_API ParticleEmitter
{
public:

  struct VRM_API Specs
  {
    static constexpr size_t s_attributeCount = 3;

    float lifeTime;
    float emitRate;

    ParticleEmitterAttribute color;
    ParticleEmitterAttribute position;
    ParticleEmitterAttribute scale;

    inline std::array<ParticleEmitterAttribute*, s_attributeCount> asArray()
    {
      return { &color, &position, &scale };
    }

    inline std::array<const ParticleEmitterAttribute*, s_attributeCount>
    asArray() const
    {
      return { &color, &position, &scale };
    }
  };

public:

  ParticleEmitter();
  ~ParticleEmitter();

  ParticleEmitter& operator=(const ParticleEmitter& other) = delete;
  ParticleEmitter(const ParticleEmitter& other)            = delete;

  ParticleEmitter& operator=(ParticleEmitter&& other) = default;
  ParticleEmitter(ParticleEmitter&& other)            = default;

  void update(const DeltaTime& dt);
  void setupRender() const;
  void executeRender(const RenderPassContext& ctx,
                     const glm::mat4*         model) const;

  inline bool isDirty() const { return m_dirtyValues; }

  void setSpecs(const Specs& specs);

  void setMesh(MeshAsset::Handle meshAsset);

  inline const Specs& getSpecs() const { return m_specs; }

  inline float        getTimeSinceStart() const { return m_timeAlive; }
  inline unsigned int getNextParticleCountToSpawn() const
  {
    return m_nextParticlesToSpawn;
  }
  inline float getNextParticleToSpawnStartingLifetime() const
  {
    return m_nextParticleSpawnLifetime;
  }

private:

  std::unique_ptr<ParticleEmitterRender> m_render;

  Specs        m_specs;
  mutable bool m_dirtyStructure = true;
  mutable bool m_dirtyValues    = true;

  float m_timeAlive                = 0.f;
  float m_lastSpawnedParticleStamp = 0.f;

  float        m_nextParticleSpawnLifetime = 0.f;
  unsigned int m_nextParticlesToSpawn      = 0;
};

} // namespace vrm
