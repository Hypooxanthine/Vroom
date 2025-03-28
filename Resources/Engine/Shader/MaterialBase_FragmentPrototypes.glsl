#ifndef __MATERIALBASE_FRAGMENTPROTOTYPES_GLSL__
#define __MATERIALBASE_FRAGMENTPROTOTYPES_GLSL__

struct PointLight
{
  float position[3];
  float color[3];
  float intensity;
  float radius;
};

struct Cluster
{
  vec4 minAABB_VS;
  vec4 maxAABB_VS;
  uint indexCount;
  uint lightIndices[100];
};

float linearizeDepth(float depth);

#endif
