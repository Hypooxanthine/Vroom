

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void UpdateParticleStates(inout ParticleStates states, float dt);
void SpawnParticle(inout ParticleStates states);

void main()
{
  const uint thread = gl_GlobalInvocationID.x;

  if (thread >= u_maxParticleCount)
  {
    return;
  }

  const uint particleIndex = thread;
  
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
    const uint spawnIndex = atomicAdd(g_spawnData.atomicCounter, 1);
    
    if (spawnIndex < u_particlesToSpawn)
    {
      SpawnParticle(states);
      const float timePerSpawn = 1.f / g_emitter.emitRate;
      updateDeltaTime = u_firstParticleStamp + float(spawnIndex) * timePerSpawn;
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
  const float t = states.ellapsedLifeTime / states.maxLifeTime;

  const vec3 position = mix(states.spawnPosition, states.deathPosition, t);
  const vec3 scale = mix(states.spawnScale, states.deathScale, t);
  const vec4 color = mix(states.spawnColor, states.deathColor, t);
  
  uint instanceIndex = atomicAdd(g_indirectCommand.instanceCount, 1);

  mat4 M = mat4(1.f);
  for (int i = 0; i < 3; ++i) M[i][i] = scale[i];

  M[3].xyz = position;

  g_particleInstanceData[instanceIndex].modelMatrix = M;
  g_particleInstanceData[instanceIndex].color = color;
}

void SpawnParticle(inout ParticleStates states)
{
  states.alive = 1;
  states.ellapsedLifeTime = 0.f;
  states.maxLifeTime = g_emitter.lifeTime;

  states.spawnPosition = g_emitter.positionSpawnValue.xyz;
  states.deathPosition = g_emitter.positionDeathValue.xyz;

  states.spawnScale = g_emitter.scaleSpawnValue.xyz;
  states.deathScale = g_emitter.scaleDeathValue.xyz;
  
  states.spawnColor = g_emitter.colorSpawnValue.xyzw;
  states.deathColor = g_emitter.colorDeathValue.xyzw;
}
