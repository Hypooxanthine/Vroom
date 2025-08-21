#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Core/DeltaTime.h"
#include "Vroom/Render/ParticleEmitterRender.h"

using namespace vrm;

ParticleEmitterAttributeBase& ParticleEmitter::Specs::getAttribute(EAttributeName name)
{
  switch (name)
  {
  case EAttributeName::eColor:
    return color;
  case EAttributeName::ePosition:
    return position;
  case EAttributeName::eScale:
    return scale;
  default:
    VRM_ASSERT_MSG(false, "Unsupported attribute type");
  }
}

const ParticleEmitterAttributeBase& ParticleEmitter::Specs::getAttribute(EAttributeName name) const
{
  return const_cast<ParticleEmitter::Specs*>(this)->getAttribute(name);
}

ParticleEmitter::ParticleEmitter()
{
  m_render.reset(new ParticleEmitterRender());
  AssetManager& manager = AssetManager::Get();
  setMesh(manager.getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"));
  m_render->setRenderMaterial(manager.getAsset<MaterialAsset>("Resources/Engine/Material/DefaultParticleMaterial.json"));
}

ParticleEmitter::~ParticleEmitter()
{
  
}

void ParticleEmitter::update(const DeltaTime& dt)
{
  m_timeAlive += dt.seconds();
  m_nextParticlesToSpawn = 0;

  const float timePerParticle = 1.f / m_specs.emitRate;
  
  if (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
  {
    m_nextParticleSpawnLifetime = std::fmod(m_lastSpawnedParticleStamp + timePerParticle, timePerParticle);

    // We need to spawn the correct amount of particles since last update
    while (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
    {
      ++m_nextParticlesToSpawn;
      m_lastSpawnedParticleStamp += timePerParticle;
    }
  }
}

void ParticleEmitter::setupRender() const
{
  if (m_dirty)
  {
    m_render->updateResources(this);
    m_dirty = false;
  }
  
  m_render->prepareFrame(this);
}

void ParticleEmitter::executeRender(const RenderPassContext &ctx) const
{
  m_render->executeRender(ctx);
}

void ParticleEmitter::setMesh(MeshAsset::Handle meshAsset)
{
  m_render->setMesh(meshAsset);
}
