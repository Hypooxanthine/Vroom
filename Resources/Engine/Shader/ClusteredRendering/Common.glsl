#ifndef __CLUSTEREDRENDEREING_COMMON_GLSL__
#define __CLUSTEREDRENDEREING_COMMON_GLSL__

struct Cluster
{
  vec4 minAABB_VS;
  vec4 maxAABB_VS;
  uint indexCount;
  uint lightIndices[100];
};

#endif
