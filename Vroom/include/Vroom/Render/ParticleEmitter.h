#pragma once

#include <cstddef>
#include <memory>
#include <span>

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

    MeshAsset::Handle mesh;

    ParticleEmitterAttribute color    = { "COLOR" };
    ParticleEmitterAttribute position = { "POSITION" };
    ParticleEmitterAttribute scale    = { "SCALE" };

    inline std::span<ParticleEmitterAttribute, s_attributeCount> getAttributes()
    {
      return std::span<ParticleEmitterAttribute, s_attributeCount>{
        &color, s_attributeCount
      };
    }

    inline std::span<ParticleEmitterAttribute const, s_attributeCount>
    getAttributes() const
    {
      return std::span<ParticleEmitterAttribute const, s_attributeCount>{
        &color, s_attributeCount
      };
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
