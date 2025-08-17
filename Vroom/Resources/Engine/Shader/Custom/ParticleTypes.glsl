#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  vec4 colorSpawn;
  vec4 colorDeath;

  vec3 positionSpawn;
  float lifeTime;
  vec3 positionDeath;
  float emitRate;

  vec3 scaleSpawn;
  vec3 scaleDeath;
};

struct EmitterSpawnData
{
  uint particlesToSpawn;
  float firstParticleStamp;
  uint atomicCounter;
};

struct ParticleStates
{
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