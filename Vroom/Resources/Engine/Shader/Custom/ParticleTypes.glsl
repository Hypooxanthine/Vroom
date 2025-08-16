#ifndef PARTICLE_TYPES_GLSL
#define PARTICLE_TYPES_GLSL

struct ParticleEmitterSpecs
{
  float lifeTime;
  float emitRate;

  vec4 color;

  vec3 initialPosition;
  vec3 initialVelocity;
  vec3 acceleration;

  vec3 initialScale;
  vec3 scaleOverTime;
};

struct EmitterData
{
  uint particlesToSpawn;
  float firstParticleStamp;
  ParticleEmitterSpecs specs;
};

struct ParticleStates
{
  vec3 position;
  vec3 velocity;
  vec3 acceleration;
  vec4 color;
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