#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  float lifeTime;
  float emitRate;
  
  vec3 spawnPosition;
  vec3 deathPosition;

  vec3 spawnScale;
  vec3 deathScale;

  vec4 spawnColor;
  vec4 deathColor;
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