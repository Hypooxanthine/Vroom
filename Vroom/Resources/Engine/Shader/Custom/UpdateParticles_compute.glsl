

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void UpdateParticleStates(inout ParticleStates states, float dt);
void SpawnParticle(inout ParticleStates states, in ParticleEmitterSpecs emitterSpecs);

void main()
{
  const uint thread = gl_GlobalInvocationID.x;

  if (thread >= u_maxParticleCount)
  {
    return;
  }

  const uint particleIndex = thread;
  
  EmitterData emitterData = g_emitters[0];
  ParticleStates states = g_particles[particleIndex];
  float updateDeltaTime = 0.f;

  if (states.alive != 0 && states.ellapsedLifeTime + u_deltaTime < states.maxLifeTime)
  {
    updateDeltaTime = u_deltaTime;
  }
  else
  {
    // Particle is not alive
    // Or will be dead after addind delta time
    const uint spawnIndex = atomicAdd(g_counters[0], 1);
    
    if (spawnIndex < emitterData.particlesToSpawn)
    {
      SpawnParticle(states, emitterData.specs);
      const float timePerSpawn = 1.f / emitterData.specs.emitRate;
      updateDeltaTime = emitterData.firstParticleStamp + float(spawnIndex) * timePerSpawn;
    }
    else
    {
      states.alive = 0;
    }
  }

  if (updateDeltaTime > 0.f)
  {
    UpdateParticleStates(states, updateDeltaTime);
  }

  g_particles[particleIndex] = states;
}

void UpdateParticleStates(inout ParticleStates states, float dt)
{
  states.ellapsedLifeTime += dt;
  states.velocity += states.acceleration * dt;
  states.position += states.velocity * dt;

  atomicAdd(g_indirectCommands[0].instanceCount, 1);
}

void SpawnParticle(inout ParticleStates states, in ParticleEmitterSpecs emitterSpecs)
{
  states.alive = 1;
  states.position = emitterSpecs.initialPosition;
  states.velocity = emitterSpecs.initialVelocity;
  states.acceleration = emitterSpecs.acceleration;
  states.maxLifeTime = emitterSpecs.lifeTime;
  states.ellapsedLifeTime = 0.f;
}
