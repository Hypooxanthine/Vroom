/**
 * @brief This compute shader is responsible for calculating the AABBs of each cluster in the cluster grid.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

#version 430 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Cluster
{
    vec4 minAABB_VS;
    vec4 maxAABB_VS;
    int indexCount;
    int indexOffset;
};

layout(std430, binding = 2) buffer ClusterInfoBlock
{
    int xCount;
    int yCount;
    int zCount;
    Cluster clusters[];
};

uniform float u_Near;
uniform float u_Far;

uniform mat4 u_InvProjection;
uniform uvec3 u_ClusterCount;

vec3 lineIntersectionWithZPlane(vec3 startPoint, vec3 endPoint, float depth);

void main()
{
    
}

// Returns the intersection point of an infinite line and a
// plane perpendicular to the Z-axis
vec3 lineIntersectionWithZPlane(vec3 startPoint, vec3 endPoint, float depth)
{
    vec3 direction = endPoint - startPoint;
    vec3 normal = vec3(0.0, 0.0, -1.0); // plane normal

    float t = (depth - dot(normal, startPoint)) / dot(normal, direction);
    return startPoint + t * direction; // the parametric form of the line equation
}