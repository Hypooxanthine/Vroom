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
    Specs();
    ~Specs() = default;

    Specs& operator=(const Specs& other);
    Specs(const Specs& other);

    Specs& operator=(Specs&& other) = default;
    Specs(Specs&& other)            = default;

    static constexpr size_t s_attributeCount = 3;

    MeshAsset::Handle mesh;

    std::unique_ptr<EmitRateEmitterAttrib> emitRate;
    std::unique_ptr<LifeTimeEmitterAttrib> lifeTime;

    std::unique_ptr<SpawnPositionEmitterAttrib> spawnPosition;
    std::unique_ptr<SpawnVelocityEmitterAttrib> spawnVelocity;
    std::unique_ptr<SpawnScaleEmitterAttrib>    spawnScale;
    std::unique_ptr<SpawnColorEmitterAttrib>    spawnColor;

    inline bool structuresDifferent(const Specs& other) const
    {
      return emitRate->structuresDifferent(*other.emitRate) || lifeTime->structuresDifferent(*other.lifeTime)
          || spawnPosition->structuresDifferent(*other.spawnPosition)
          || spawnVelocity->structuresDifferent(*other.spawnVelocity)
          || spawnScale->structuresDifferent(*other.spawnScale) || spawnColor->structuresDifferent(*other.spawnColor);
    }

    size_t computeStatesRequiredSize() const;
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
  void executeRender(const RenderPassContext& ctx, const glm::mat4* model) const;

  inline bool isDirty() const { return m_dirtyValues; }

  void setSpecs(Specs&& specs);

  inline const Specs& getSpecs() const { return m_specs; }

  inline float getTimeSinceStart() const { return m_timeAlive; }

  /**
   * @brief Get the amount of particles that need to be spawned on this frame
   *
   * @return unsigned int
   */
  inline unsigned int getNextParticleCountToSpawn() const { return m_nextParticlesToSpawn; }

  /**
   * @brief Get the elapsed lifetime of the first particle that need to be spawned on this frame
   *
   * @return float
   */
  inline float getNextParticleToSpawnStartingLifetime() const { return m_nextParticleSpawnLifetime; }

private:

  std::unique_ptr<ParticleEmitterRender> m_render;

  Specs        m_specs;
  mutable bool m_dirtyStructure = true;
  mutable bool m_dirtyValues    = true;

  float m_timeAlive                = 0.f;
  float m_lastSpawnedParticleStamp = 0.f;

  float m_nextParticleSpawnLifetime = 0.f;

  // The amount of particles that need to be spawned by the
  // compute shader on this frame
  unsigned int m_nextParticlesToSpawn = 0;
};

} // namespace vrm
