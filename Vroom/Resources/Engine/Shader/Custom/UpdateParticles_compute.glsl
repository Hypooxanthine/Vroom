

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void UpdateParticleStates(inout ParticleStates states, in ParticleEmitterSpecs specs, float dt);
void SpawnParticle(inout ParticleStates states, in ParticleEmitterSpecs emitterSpecs);

void main()
{
  const uint thread = gl_GlobalInvocationID.x;

  if (thread >= u_maxParticleCount)
  {
    return;
  }

  const uint particleIndex = thread;
  
  ParticleEmitterSpecs emitterSpecs = g_emitters[0];
  EmitterSpawnData emitterSpawnData = g_spawnData[0];
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
    const uint spawnIndex = atomicAdd(g_spawnData[0].atomicCounter, 1);
    
    if (spawnIndex < emitterSpawnData.particlesToSpawn)
    {
      SpawnParticle(states, emitterSpecs);
      const float timePerSpawn = 1.f / emitterSpecs.emitRate;
      updateDeltaTime = emitterSpawnData.firstParticleStamp + float(spawnIndex) * timePerSpawn;
    }
    else
    {
      states.alive = 0;
    }
  }

  if (updateDeltaTime > 0.f)
  {
    UpdateParticleStates(states, emitterSpecs, updateDeltaTime);
  }

  g_particles[particleIndex] = states;
}

void UpdateParticleStates(inout ParticleStates states, in ParticleEmitterSpecs specs, float dt)
{
  states.ellapsedLifeTime += dt;
  states.velocity += states.acceleration * dt;
  states.position += states.velocity * dt;
  states.scale *= (1.0 + specs.scaleOverTime * dt);
  
  uint instanceIndex = atomicAdd(g_indirectCommands[0].instanceCount, 1);

  mat4 M = mat4(1.f);
  for (int i = 0; i < 3; ++i) M[i][i] = states.scale[i];

  M[3].xyz = states.position;

  g_particleInstanceData[instanceIndex].modelMatrix = M;
  // g_particleInstanceData[instanceIndex].modelMatrix[3].xyz = states.position;
  g_particleInstanceData[instanceIndex].color = states.color;
}

void SpawnParticle(inout ParticleStates states, in ParticleEmitterSpecs emitterSpecs)
{
  states.alive = 1;
  states.position = emitterSpecs.initialPosition;
  states.velocity = emitterSpecs.initialVelocity;
  states.acceleration = emitterSpecs.acceleration;
  states.maxLifeTime = emitterSpecs.lifeTime;
  states.scale = emitterSpecs.initialScale;
  states.color = emitterSpecs.color;
  states.ellapsedLifeTime = 0.f;
}
