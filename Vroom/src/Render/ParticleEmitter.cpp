#include "Vroom/Render/ParticleEmitter.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/DeltaTime.h"

using namespace vrm;

ParticleEmitter::ParticleEmitter()
{
  m_render.reset(new ParticleEmitterRender());
  AssetManager& manager = AssetManager::Get();
  setMesh(
    manager.getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"));
  m_render->setRenderMaterial(manager.getAsset<MaterialAsset>(
    "Resources/Engine/Material/DefaultParticleMaterial.json"));
}

ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::update(const DeltaTime& dt)
{
  m_timeAlive            += dt.seconds();
  m_nextParticlesToSpawn  = 0;

  const float timePerParticle = 1.f / m_specs.emitRate;

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

void ParticleEmitter::setSpecs(const Specs& specs)
{
  m_dirtyValues = true;

  const auto& currentAttribs = m_specs.getAttributes();
  const auto& newAttribs     = specs.getAttributes();

  for (size_t i = 0; i < Specs::s_attributeCount; ++i)
  {
    if (currentAttribs[i].structureDifferent(newAttribs[i]))
    {
      m_dirtyStructure = true;
      break;
    }
  }

  m_specs = specs;
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

void ParticleEmitter::setMesh(MeshAsset::Handle meshAsset)
{
  m_render->setMesh(meshAsset);
}
