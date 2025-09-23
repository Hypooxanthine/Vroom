

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void UpdateParticleStates(inout ParticleStates states, float dt);
void SpawnParticle(inout ParticleStates states);
vec4 GenerateRandomRange4(in vec4 min, in vec4 max);
vec3 GenerateRandomRange3(in vec3 min, in vec3 max);
uint murmurHash(in uint x);

// Position, scale, color
const uint g_attributeCount = 3;

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

// Position attribute
  // Spawn field
#ifdef VRM_PARTICLE_POSITION_SPAWN_CONST_FIELD
  states.spawnPosition = g_emitter.positionSpawnValue.xyz;
#endif
#ifdef VRM_PARTICLE_POSITION_SPAWN_RANDOM_RANGE_FIELD
  states.spawnPosition = GenerateRandomRange3(g_emitter.positionSpawnMinValue.xyz, g_emitter.positionSpawnMaxValue.xyz);
#endif

// Death field
#ifdef VRM_PARTICLE_POSITION_DEATH_CONST_FIELD
  states.deathPosition = g_emitter.positionDeathValue.xyz;
#endif
#ifdef VRM_PARTICLE_POSITION_DEATH_RANDOM_RANGE_FIELD
  states.deathPosition = GenerateRandomRange3(g_emitter.positionDeathMinValue.xyz, g_emitter.positionDeathMaxValue.xyz);
#endif

// Scale attribute
  // Spawn field
#ifdef VRM_PARTICLE_SCALE_SPAWN_CONST_FIELD
  states.spawnScale = g_emitter.scaleSpawnValue.xyz;
#endif
#ifdef VRM_PARTICLE_SCALE_SPAWN_RANDOM_RANGE_FIELD
  states.spawnScale = GenerateRandomRange3(g_emitter.scaleSpawnMinValue.xyz, g_emitter.scaleSpawnMaxValue.xyz);
#endif

// Death field
#ifdef VRM_PARTICLE_SCALE_DEATH_CONST_FIELD
  states.deathScale = g_emitter.scaleDeathValue.xyz;
#endif
#ifdef VRM_PARTICLE_SCALE_DEATH_RANDOM_RANGE_FIELD
  states.deathScale = GenerateRandomRange3(g_emitter.scaleDeathMinValue.xyz, g_emitter.scaleDeathMaxValue.xyz);
#endif

// Color attribute
  // Spawn field
#ifdef VRM_PARTICLE_COLOR_SPAWN_CONST_FIELD
  states.spawnColor = g_emitter.colorSpawnValue.xyzw;
#endif
#ifdef VRM_PARTICLE_COLOR_SPAWN_RANDOM_RANGE_FIELD
  states.spawnColor = GenerateRandomRange4(g_emitter.colorSpawnMinValue, g_emitter.colorSpawnMaxValue);
#endif

// Death field
#ifdef VRM_PARTICLE_COLOR_DEATH_CONST_FIELD
  states.deathColor = g_emitter.colorDeathValue.xyzw;
#endif
#ifdef VRM_PARTICLE_COLOR_DEATH_RANDOM_RANGE_FIELD
  states.deathColor = GenerateRandomRange4(g_emitter.colorDeathMinValue, g_emitter.colorDeathMaxValue);
#endif
}

uint g_rndCounter = 0;

vec4 GenerateRandomRange4(in vec4 min, in vec4 max)
{
  vec4 rnd;

  for (uint i = 0; i < 4; ++i)
  {
    uint hash = murmurHash(u_randomSeed + gl_GlobalInvocationID.x * g_attributeCount + g_rndCounter++);
    float normalized = float(hash) / float(0xFFFFFFFFu);
    rnd[i] = mix(min[i], max[i], normalized);
  }
   
  return rnd;
}

vec3 GenerateRandomRange3(in vec3 min, in vec3 max)
{
  vec3 rnd;

  for (uint i = 0; i < 3; ++i)
  {
    uint hash = murmurHash(u_randomSeed + gl_GlobalInvocationID.x * g_attributeCount + g_rndCounter++);
    float normalized = float(hash) / float(0xFFFFFFFFu);
    rnd[i] = mix(min[i], max[i], normalized);
  }
   
  return rnd;
}

uint murmurHash(in uint x)
{
  x ^= x >> 16;
  x *= 0x7feb352dU;
  x ^= x >> 15;
  x *= 0x846ca68bU;
  x ^= x >> 16;
  return x;
}
