#ifndef _CULLING_CUMPUTE_GLSL_
#define _CULLING_CUMPUTE_GLSL_

/**
 * @brief This compute shader is responsible for assigning lights to clusters.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

layout(local_size_x = LOCAL_SIZE_X, local_size_y = LOCAL_SIZE_Y, local_size_z = LOCAL_SIZE_Z) in;

bool testSphereAABB(uint i, Cluster c);

// each invocation of main() is a thread processing a cluster
void main()
{
  uint xCount = ClusterInfoBlock_xCount;
  uint yCount = ClusterInfoBlock_yCount;
  uint zCount = ClusterInfoBlock_zCount;

  uint x = gl_GlobalInvocationID.x, y = gl_GlobalInvocationID.y, z = gl_GlobalInvocationID.z;
  uint clusterIndex = x + y * xCount + z * xCount * yCount;
  Cluster cluster = ClusterInfoBlock_clusters[clusterIndex];

  // we need to reset count because culling runs every frame.
  // otherwise it would accumulate.
  cluster.indexCount = 0;

  for (uint i = 0; i < LightBlock_pointLightCount; ++i)
  {
    if (testSphereAABB(i, cluster) && cluster.indexCount < 100)
    {
      cluster.lightIndices[cluster.indexCount] = i;
      cluster.indexCount++;
    }
  }
  ClusterInfoBlock_clusters[clusterIndex] = cluster;
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
  PointLight pointLight = LightBlock_pointLights[i];
  vec3 center = vec3(u_View * pointLight.position);
  float radius = pointLight.radius;

  vec3 aabbMin = cluster.minAABB_VS.xyz;
  vec3 aabbMax = cluster.maxAABB_VS.xyz;

  return sphereAABBIntersection(center, radius, aabbMin, aabbMax);
}

#endif
