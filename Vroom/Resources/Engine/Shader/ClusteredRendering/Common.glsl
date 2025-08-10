#ifndef _CLUSTEREDRENDEREING_COMMON_GLSL_
#define _CLUSTEREDRENDEREING_COMMON_GLSL_

struct ClusterHeader
{
  uint xCount;
  uint yCount;
  uint zCount;
  uint _pad;
};

struct Cluster
{
  vec4 minAABB_VS;
  vec4 maxAABB_VS;
  uint indexCount;
  uint lightIndices[103];
};

#endif
