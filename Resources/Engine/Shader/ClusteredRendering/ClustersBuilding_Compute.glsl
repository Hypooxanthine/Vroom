/**
 * @brief This compute shader is responsible for calculating the AABBs of each cluster in the cluster grid.
 * Thanks to https://github.com/DaveH355/clustered-shading for his tutorial on cluster shading.
 */

#version 430 core
layout(local_size_x = LOCAL_SIZE_X, local_size_y = LOCAL_SIZE_Y, local_size_z = LOCAL_SIZE_Z) in;

uniform mat4 u_InvProjection;

vec3 intersectionLineAndZPerpendicularPlane(vec3 linePoint, vec3 lineDirection, float depth);

void main()
{
  uint x = gl_GlobalInvocationID.x, y = gl_GlobalInvocationID.y, z = gl_GlobalInvocationID.z;
  uint clusterIndex = x + y * xCount + z * xCount * yCount;

  vec2 clusterSize_NDC_xy = { 2.0 / xCount, 2.0 / yCount };

  vec4 nearBottomLeft_NDC = vec4(
    -1.0 + x * clusterSize_NDC_xy.x,
    -1.0 + y * clusterSize_NDC_xy.y,
    -1.0, // We'll deal with depth value later
    1.0
  );

  vec4 farTopRight_NDC = vec4(
    -1.0 + (x + 1) * clusterSize_NDC_xy.x,
    -1.0 + (y + 1) * clusterSize_NDC_xy.y,
    -1.0, // We'll deal with depth value later
    1.0
  );

  vec4 nearBottomLeft4_VS = u_InvProjection * nearBottomLeft_NDC;
  vec3 nearBottomLeft_VS = (nearBottomLeft4_VS / nearBottomLeft4_VS.w).xyz;
  vec4 farTopRight4_VS = u_InvProjection * farTopRight_NDC;
  vec3 farTopRight_VS = (farTopRight4_VS / farTopRight4_VS.w).xyz;

  // Now we need to find the depth values of this cluster.
  float nearDepth = u_Near * pow(u_Far / u_Near, float(z) / zCount);
  float farDepth = u_Near * pow(u_Far / u_Near, float(z + 1) / zCount);

  // Now we need to find the intersection from the line eyePos -> AABB corners to near and far z planes.
  vec3 nearMin = intersectionLineAndZPerpendicularPlane(
    vec3(0.0), // View origin
    vec3(nearBottomLeft_VS), // direction is origin -> corner
    nearDepth
  );
  vec3 nearMax = intersectionLineAndZPerpendicularPlane(
    vec3(0.0), // View origin
    vec3(farTopRight_VS), // direction is origin -> corner
    nearDepth
  );
  vec3 farMin = intersectionLineAndZPerpendicularPlane(
    vec3(0.0), // View origin
    vec3(nearBottomLeft_VS), // direction is origin -> corner
    farDepth
  );
  vec3 farMax = intersectionLineAndZPerpendicularPlane(
    vec3(0.0), // View origin
    vec3(farTopRight_VS), // direction is origin -> corner
    farDepth
  );

  clusters[clusterIndex].minAABB_VS = vec4(min(nearMin, farMin), 1.0);
  clusters[clusterIndex].maxAABB_VS = vec4(max(nearMax, farMax), 1.0);
}

vec3 intersectionLineAndZPerpendicularPlane(vec3 linePoint, vec3 lineDirection, float depth)
{
  vec3 planeNormal = vec3(0.0, 0.0, -1.0);
  return linePoint + ( (depth - dot(planeNormal, linePoint)) / dot(planeNormal, lineDirection) ) * lineDirection;
}