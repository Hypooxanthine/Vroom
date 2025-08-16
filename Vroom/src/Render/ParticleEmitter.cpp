#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/DeltaTime.h"

using namespace vrm;

ParticleEmitter::ParticleEmitter()
{
  AssetManager& manager = AssetManager::Get();
  setMesh(manager.getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"));
  setMaterial(manager.getAsset<MaterialAsset>("Resources/Engine/Material/DefaultParticleMaterial.json"));
}

ParticleEmitter::~ParticleEmitter()
{
  
}

ParticleEmitter::ParticleEmitter(MeshAsset::Handle meshAsset, MaterialAsset::Handle materialAsset)
{
  setMesh(meshAsset);
  setMaterial(materialAsset);
}

void ParticleEmitter::update(const DeltaTime& dt)
{
  m_timeAlive += dt.seconds();
  m_nextParticlesToSpawn = 0;

  const float timePerParticle = 1.f / m_specs.emitRate;
  
  if (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
  {
    m_nextParticleSpawnLifetime = m_lastSpawnedParticleStamp + timePerParticle;

    // We need to spawn the correct amount of particles since last update
    while (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
    {
      ++m_nextParticlesToSpawn;
      m_lastSpawnedParticleStamp += timePerParticle;
    }
  }
}
