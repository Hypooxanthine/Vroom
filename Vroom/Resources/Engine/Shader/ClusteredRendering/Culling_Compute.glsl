#ifndef _CULLING_CUMPUTE_GLSL_
#define _CULLING_CUMPUTE_GLSL_

/**
 * @brief This compute shader is responsible for assigning lights to clusters.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

bool testSphereAABB(uint i, Cluster c);

void main()
{
  uint xCount = g_clusterHeader.xCount;
  uint yCount = g_clusterHeader.yCount;
  uint zCount = g_clusterHeader.zCount;

  uint clusterIndex = gl_GlobalInvocationID.x;

  if (clusterIndex > xCount * yCount * zCount)
    return;

  Cluster cluster = g_clusters[clusterIndex];

  // we need to reset count because culling runs every frame.
  // otherwise it would accumulate.
  cluster.indexCount = 0;

  for (uint i = 0; i < g_pointLightCount; ++i)
  {
    if (testSphereAABB(i, cluster) && cluster.indexCount < 100)
    {
      cluster.lightIndices[cluster.indexCount] = i;
      cluster.indexCount++;
    }
  }
  g_clusters[clusterIndex] = cluster;
}

bool sphereAABBIntersection(vec3 center, float radius, vec3 aabbMin, vec3 aabbMax)
{
  // closest point on the AABB to the sphere center
  vec3 closestPoint = clamp(center, aabbMin, aabbMax);
  // squared distance between the sphere center and closest point
  float distanceSquared = dot(closestPoint - center, closestPoint - center);
  return distanceSquared <= radius * radius;
}

// this just unpacks data for sphereAABBIntersection
bool testSphereAABB(uint i, Cluster cluster)
{
  PointLight pointLight = g_pointLights[i];
  vec3 center = vec3(u_View * pointLight.position);
  float radius = pointLight.radius;

  vec3 aabbMin = cluster.minAABB_VS.xyz;
  vec3 aabbMax = cluster.maxAABB_VS.xyz;

  return sphereAABBIntersection(center, radius, aabbMin, aabbMax);
}

#endif
