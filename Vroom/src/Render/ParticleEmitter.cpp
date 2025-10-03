#include "Vroom/Render/ParticleEmitter.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/DeltaTime.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "glm/fwd.hpp"

using namespace vrm;

ParticleEmitter::Specs::Specs()
{
  emitRate      = std::make_unique<EmitRateEmitterAttrib>();
  lifeTime      = std::make_unique<LifeTimeEmitterAttrib>();
  spawnPosition = std::make_unique<SpawnPositionEmitterAttrib>();
  spawnVelocity = std::make_unique<SpawnVelocityEmitterAttrib>();
  spawnScale    = std::make_unique<SpawnScaleEmitterAttrib>();
  spawnColor    = std::make_unique<SpawnColorEmitterAttrib>();
}

ParticleEmitter::Specs& ParticleEmitter::Specs::operator=(const Specs& other)
{
  if (this != &other)
  {
    mesh = other.mesh;
    emitRate.reset(other.emitRate->clone());
    lifeTime.reset(other.lifeTime->clone());
    spawnPosition.reset(other.spawnPosition->clone());
    spawnVelocity.reset(other.spawnVelocity->clone());
    spawnScale.reset(other.spawnScale->clone());
    spawnColor.reset(other.spawnColor->clone());
  }

  return *this;
}

ParticleEmitter::Specs::Specs(const Specs& other) { *this = other; }

size_t ParticleEmitter::Specs::computeStatesRequiredSize() const
{
  render::SSBO430Layout layout;

  layout.push<glm::uint>();    // Alive
  layout.push<glm::float32>(); // Ellapsed life time
  layout.push<glm::float32>(); // Max life time
  layout.push<glm::vec3>();    // Current position
  layout.push<glm::vec3>();    // Current velocity
  layout.push<glm::vec3>();    // Current scale
  layout.push<glm::vec4>();    // Current color

  return layout.getAlignedSize();
}

ParticleEmitter::ParticleEmitter()
{
  m_render.reset(new ParticleEmitterRender());
  AssetManager& manager = AssetManager::Get();
  m_render->setRenderMaterial(manager.getAsset<MaterialAsset>(
    "Resources/Engine/Material/DefaultParticleMaterial.json"));
}

ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::update(const DeltaTime& dt)
{
  m_timeAlive            += dt.seconds();
  m_nextParticlesToSpawn  = 0;

  const float timePerParticle = 1.f / m_specs.emitRate->getEmitRate();

  if (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
  {
    m_nextParticleSpawnLifetime =
      std::fmod(m_lastSpawnedParticleStamp + timePerParticle, timePerParticle);

    // We need to spawn the correct amount of particles since last update
    while (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
    {
      ++m_nextParticlesToSpawn;
      m_lastSpawnedParticleStamp += timePerParticle;
    }
  }
}

void ParticleEmitter::setSpecs(Specs&& specs)
{
  m_dirtyValues    = true;
  m_dirtyStructure = m_dirtyStructure || m_specs.structuresDifferent(specs);

  m_specs = std::move(specs);
}

void ParticleEmitter::setupRender() const
{
  if (m_dirtyStructure)
  {
    m_render->rebuildMaterials(*this);
    m_dirtyStructure = false;
  }
  if (m_dirtyValues)
  {
    m_render->setMesh(m_specs.mesh);
    m_render->updateResources(*this);
    m_dirtyValues = false;
  }

  m_render->prepareFrame(*this);
}

void ParticleEmitter::executeRender(const RenderPassContext& ctx,
                                    const glm::mat4*         model) const
{
  m_render->executeRender(*this, ctx, model);
}
