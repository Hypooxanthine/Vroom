#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

#define VRM_DECLARE_SPAWN_CONST_FIELD(attributeName) \
  vec4 attributeName ## SpawnValue

#define VRM_DECLARE_DEATH_CONST_FIELD(attributeName) \
  vec4 attributeName ## DeathValue

#define VRM_DECLARE_SPAWN_RANDOM_RANGE_FIELD(attributeName) \
  vec4 attributeName ## SpawnMinValue; \
  vec4 attributeName ## SpawnMaxValue

#define VRM_DECLARE_DEATH_RANDOM_RANGE_FIELD(attributeName) \
  vec4 attributeName ## DeathMinValue; \
  vec4 attributeName ## DeathMaxValue

struct ParticleEmitterSpecs
{
  float lifeTime;
  float emitRate;

  // Position attribute
    // Spawn field
  #ifdef VRM_PARTICLE_POSITION_SPAWN_CONST_FIELD
    VRM_DECLARE_SPAWN_CONST_FIELD(position);
  #endif
  #ifdef VRM_PARTICLE_POSITION_SPAWN_RANDOM_RANGE_FIELD
    VRM_DECLARE_SPAWN_RANDOM_RANGE_FIELD(position);
  #endif

  // Death field
  #ifdef VRM_PARTICLE_POSITION_DEATH_CONST_FIELD
    VRM_DECLARE_DEATH_CONST_FIELD(position);
  #endif
  #ifdef VRM_PARTICLE_POSITION_DEATH_RANDOM_RANGE_FIELD
    VRM_DECLARE_DEATH_RANDOM_RANGE_FIELD(position);
  #endif

  // Scale attribute
    // Spawn field
  #ifdef VRM_PARTICLE_SCALE_SPAWN_CONST_FIELD
    VRM_DECLARE_SPAWN_CONST_FIELD(scale);
  #endif
  #ifdef VRM_PARTICLE_SCALE_SPAWN_RANDOM_RANGE_FIELD
    VRM_DECLARE_SPAWN_RANDOM_RANGE_FIELD(scale);
  #endif

  // Death field
  #ifdef VRM_PARTICLE_SCALE_DEATH_CONST_FIELD
    VRM_DECLARE_DEATH_CONST_FIELD(scale);
  #endif
  #ifdef VRM_PARTICLE_SCALE_DEATH_RANDOM_RANGE_FIELD
    VRM_DECLARE_DEATH_RANDOM_RANGE_FIELD(scale);
  #endif

  // Color attribute
    // Spawn field
  #ifdef VRM_PARTICLE_COLOR_SPAWN_CONST_FIELD
    VRM_DECLARE_SPAWN_CONST_FIELD(color);
  #endif
  #ifdef VRM_PARTICLE_COLOR_SPAWN_RANDOM_RANGE_FIELD
    VRM_DECLARE_SPAWN_RANDOM_RANGE_FIELD(color);
  #endif

  // Death field
  #ifdef VRM_PARTICLE_COLOR_DEATH_CONST_FIELD
    VRM_DECLARE_DEATH_CONST_FIELD(color);
  #endif
  #ifdef VRM_PARTICLE_COLOR_DEATH_RANDOM_RANGE_FIELD
    VRM_DECLARE_DEATH_RANDOM_RANGE_FIELD(color);
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

  vec3 spawnPosition;
  vec3 deathPosition;
  vec4 spawnColor;
  vec4 deathColor;
  vec3 spawnScale;
  vec3 deathScale;
};

struct ParticleInstanceData
{
  mat4 modelMatrix;
  vec4 color;
};

#endif // PARTICLE_TYPES_GLSL