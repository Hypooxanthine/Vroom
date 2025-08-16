#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  vec4 color;

  vec3 initialPosition;
  float lifeTime;
  vec3 initialVelocity;
  float emitRate;

  vec3 acceleration;
  vec3 initialScale;
  vec3 scaleOverTime;
};

struct EmitterSpawnData
{
  uint particlesToSpawn;
  float firstParticleStamp;
  uint atomicCounter;
};

struct ParticleStates
{
  vec3 position;
  float ellapsedLifeTime;
  
  vec3 velocity;
  float maxLifeTime;

  vec3 acceleration;
  uint alive;

  vec3 scale;

  vec4 color;
};

struct ParticleInstanceData
{
  mat4 modelMatrix;
  vec4 color;
};

#endif // PARTICLE_TYPES_GLSL