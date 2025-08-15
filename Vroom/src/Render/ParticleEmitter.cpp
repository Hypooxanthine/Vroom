#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Core/DeltaTime.h"

using namespace vrm;

ParticleEmitter::ParticleEmitter()
{

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
    m_nextParticleSpawnLifetime = m_lastSpawnedParticleStamp + timePerParticle;

    // We need to spawn the correct amount of particles since last update
    while (m_lastSpawnedParticleStamp + timePerParticle < m_timeAlive)
    {
      ++m_nextParticlesToSpawn;
      m_lastSpawnedParticleStamp += timePerParticle;
    }
  }
}
