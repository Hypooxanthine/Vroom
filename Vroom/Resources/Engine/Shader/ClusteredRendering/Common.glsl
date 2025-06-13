#ifndef _CLUSTEREDRENDEREING_COMMON_GLSL_
#define _CLUSTEREDRENDEREING_COMMON_GLSL_

struct Cluster
{
  vec4 minAABB_VS;
  vec4 maxAABB_VS;
  uint indexCount;
  uint lightIndices[100];
};

#endif
