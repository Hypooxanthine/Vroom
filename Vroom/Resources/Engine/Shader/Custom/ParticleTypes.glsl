#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  vec4 spawnColor;
  vec4 deathColor;

  vec3 spawnPosition;
  float lifeTime;
  vec3 deathPosition;
  float emitRate;

  vec3 spawnScale;
  vec3 deathScale;
};

struct EmitterSpawnData
{
  uint atomicCounter;
};

struct ParticleStates
{
  vec3 spawnPosition;
  vec3 deathPosition;
  vec4 spawnColor;
  vec4 deathColor;
  vec3 spawnScale;
  vec3 deathScale;

  uint alive;
  float ellapsedLifeTime;
  float maxLifeTime;
};

struct ParticleInstanceData
{
  mat4 modelMatrix;
  vec4 color;
};

#endif // PARTICLE_TYPES_GLSL