#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  // EmitRate attribute : only const field is supported
  float emitRate;
  
  // LifeTime attribute
#ifdef VRM_PARTICLE_LifeTime_Const
  float lifeTime;
#endif
#ifdef VRM_PARTICLE_LifeTime_RandomRange
  float lifeTimeMin;
  float lifeTimeMax;
#endif

  // Position attribute
#ifdef VRM_PARTICLE_SpawnPosition_Const
  vec3 spawnPosition;
#endif
#ifdef VRM_PARTICLE_SpawnPosition_RandomRange
  vec3 spawnPositionMin;
  vec3 spawnPositionMax;
#endif

  // Velocity attribute
#ifdef VRM_PARTICLE_SpawnVelocity_Const
  vec3 spawnVelocity;
#endif
#ifdef VRM_PARTICLE_SpawnVelocity_RandomRange
  vec3 spawnVelocityMin;
  vec3 spawnVelocityMax;
#endif

  // Scale attribute
#ifdef VRM_PARTICLE_SpawnScale_Const
  vec3 spawnScale;
#endif
#ifdef VRM_PARTICLE_SpawnScale_RandomRange
  vec3 spawnScaleMin;
  vec3 spawnScaleMax;
#endif

  // Color attribute
#ifdef VRM_PARTICLE_SpawnColor_Const
  vec4 spawnColor;
#endif
#ifdef VRM_PARTICLE_SpawnColor_RandomRange
  vec4 spawnColorMin;
  vec4 spawnColorMax;
#endif

};

struct EmitterSpawnData
{
  uint atomicCounter;
};

struct ParticleStates
{
  uint alive;
  float ellapsedLifeTime;
  float maxLifeTime;

  vec3 position;
  vec3 velocity;
  vec3 scale;
  vec4 color;
};

struct ParticleInstanceData
{
  mat4 modelMatrix;
  vec4 color;
};

#endif // PARTICLE_TYPES_GLSL