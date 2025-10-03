

layout (local_size_x = VRM_UPDATER_GROUP_SIZE) in;

void UpdateParticleStates(inout ParticleStates states, float dt);
void SpawnParticle(inout ParticleStates states);
vec4 GenerateRandomRange4(in vec4 min, in vec4 max);
vec3 GenerateRandomRange3(in vec3 min, in vec3 max);
float GenerateRandomRange1(in float min, in float max);
uint murmurHash(in uint x);

// Lifetime, position, scale, color
const uint g_attributeCount = 4;

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
  
  states.position += states.velocity * dt;
  
  uint instanceIndex = atomicAdd(g_indirectCommand.instanceCount, 1);

  mat4 M = mat4(1.f);
  for (int i = 0; i < 3; ++i) M[i][i] = states.scale[i];

  M[3].xyz = states.position;

  g_particleInstanceData[instanceIndex].modelMatrix = M;
  g_particleInstanceData[instanceIndex].color = states.color;
}

void SpawnParticle(inout ParticleStates states)
{
  states.alive = 1;
  states.ellapsedLifeTime = 0.f;
  states.velocity = vec3(0.f);
  
  // LifeTime attribute
#ifdef VRM_PARTICLE_LifeTime_Const
  states.maxLifeTime = g_emitter.lifeTime;
#endif
#ifdef VRM_PARTICLE_LifeTime_RandomRange
  states.maxLifeTime = GenerateRandomRange1(g_emitter.lifeTimeMin, g_emitter.lifeTimeMax);
#endif

  // Position attribute
#ifdef VRM_PARTICLE_SpawnPosition_Const
  states.position = g_emitter.spawnPosition;
#endif
#ifdef VRM_PARTICLE_SpawnPosition_RandomRange
  states.position = GenerateRandomRange3(g_emitter.spawnPositionMin, g_emitter.spawnPositionMax);
#endif

  // Velocity attribute
#ifdef VRM_PARTICLE_SpawnVelocity_Const
  states.velocity = g_emitter.spawnVelocity;
#endif
#ifdef VRM_PARTICLE_SpawnVelocity_RandomRange
  states.velocity = GenerateRandomRange3(g_emitter.spawnVelocityMin, g_emitter.spawnVelocityMax);
#endif

  // Scale attribute
#ifdef VRM_PARTICLE_SpawnScale_Const
  states.scale = g_emitter.spawnScale;
#endif
#ifdef VRM_PARTICLE_SpawnScale_RandomRange
  states.scale = GenerateRandomRange3(g_emitter.spawnScaleMin, g_emitter.spawnScaleMax);
#endif

  // Color attribute
#ifdef VRM_PARTICLE_SpawnColor_Const
  states.color = g_emitter.spawnColor;
#endif
#ifdef VRM_PARTICLE_SpawnColor_RandomRange
  states.color = GenerateRandomRange4(g_emitter.spawnColorMin, g_emitter.spawnColorMax);
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

float GenerateRandomRange1(in float min, in float max)
{
  uint hash = murmurHash(u_randomSeed + gl_GlobalInvocationID.x * g_attributeCount + g_rndCounter++);
  float normalized = float(hash) / float(0xFFFFFFFFu);
  return mix(min, max, normalized);
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
