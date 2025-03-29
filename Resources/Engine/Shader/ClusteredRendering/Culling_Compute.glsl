/**
 * @brief This compute shader is responsible for assigning lights to clusters.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

#version 430 core

layout(local_size_x = LOCAL_SIZE_X, local_size_y = LOCAL_SIZE_Y, local_size_z = LOCAL_SIZE_Z) in;

bool testSphereAABB(uint i, Cluster c);

// each invocation of main() is a thread processing a cluster
void main()
{
  uint x = gl_GlobalInvocationID.x, y = gl_GlobalInvocationID.y, z = gl_GlobalInvocationID.z;
  uint clusterIndex = x + y * xCount + z * xCount * yCount;
  Cluster cluster = clusters[clusterIndex];

  // we need to reset count because culling runs every frame.
  // otherwise it would accumulate.
  cluster.indexCount = 0;

  for (uint i = 0; i < pointLightCount; ++i)
  {
    if (testSphereAABB(i, cluster) && cluster.indexCount < 100)
    {
      cluster.lightIndices[cluster.indexCount] = i;
      cluster.indexCount++;
    }
  }
  clusters[clusterIndex] = cluster;
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
  vec3 center = vec3(u_View * vec4(pointLights[i].position[0], pointLights[i].position[1], pointLights[i].position[2], 1.0));
  float radius = pointLights[i].radius;

  vec3 aabbMin = cluster.minAABB_VS.xyz;
  vec3 aabbMax = cluster.maxAABB_VS.xyz;

  return sphereAABBIntersection(center, radius, aabbMin, aabbMax);
}