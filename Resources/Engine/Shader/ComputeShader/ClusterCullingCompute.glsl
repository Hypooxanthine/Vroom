/**
 * @brief This compute shader is responsible for assigning lights to clusters.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

#version 430 core

#define LOCAL_SIZE 128
layout(local_size_x = LOCAL_SIZE, local_size_y = 1, local_size_z = 1) in;

struct PointLight
{
    float position[3];
    float color[3];
    float intensity;
    float radius;
};

layout(std430, binding = 0) buffer LightBlock
{
    uint pointLightCount;
    PointLight pointLights[];
};

struct Cluster
{
    vec4 minAABB_VS;
    vec4 maxAABB_VS;
    uint indexCount;
    uint lightIndices[100];
};

layout(std430, binding = 1) buffer ClusterInfoBlock
{
    uint xCount;
    uint yCount;
    uint zCount;
    Cluster clusters[];
};

uniform mat4 u_View;

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