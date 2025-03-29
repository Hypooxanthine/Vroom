#ifndef __MATERIALBASE_FRAGMENTPROTOTYPES_GLSL__
#define __MATERIALBASE_FRAGMENTPROTOTYPES_GLSL__

float linearizeDepth(float depth);

vec3 g_clusterIndex;

void SetupGlobalVars_ClusteredRendering()
{
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
  return clusters[clusterIndex].indexCount;
}

PointLight GetRelevantPointLight_ClusteredRendering(in uint index)
{
  uint clusterIndex = GetClusterIndex();
  uint lightIndex = clusters[clusterIndex].lightIndices[index];
  return LightBlock_pointLights[lightIndex];
}

#endif
