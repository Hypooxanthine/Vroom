#ifndef _MATERIALBASE_FRAGMENTPROTOTYPES_GLSL_
#define _MATERIALBASE_FRAGMENTPROTOTYPES_GLSL_

float linearizeDepth(float depth);

uint g_clusterIndex;

void SetupGlobalVars_ClusteredRendering()
{
  uint xCount = ClusterInfoBlock_xCount;
  uint yCount = ClusterInfoBlock_yCount;
  uint zCount = ClusterInfoBlock_zCount;

  // Coordinates of the frag in VS for finding the right cluster
  uint zCoord = uint((log(abs(v_CameraDepth) / u_Near) * zCount) / log(u_Far / u_Near));
  vec2 clusterSizeXY = vec2(u_ViewportSize) / vec2(xCount, yCount);

  // Coordinates of the frag in NDC space for finding the right cluster
  uvec3 clusterCoords = ivec3(gl_FragCoord.xy / clusterSizeXY, zCoord);
  g_clusterIndex = clusterCoords.z * (yCount * xCount) + clusterCoords.y * (xCount) + clusterCoords.x;
}

uint GetClusterIndex()
{
  return g_clusterIndex;
}

uint GetRelevantPointLightCount_ClusteredRendering()
{
  uint clusterIndex = GetClusterIndex();
  return ClusterInfoBlock_clusters[clusterIndex].indexCount;
}

PointLight GetRelevantPointLight_ClusteredRendering(in uint index)
{
  uint clusterIndex = GetClusterIndex();
  uint lightIndex = ClusterInfoBlock_clusters[clusterIndex].lightIndices[index];
  return LightBlock_pointLights[lightIndex];
}

#endif
