#ifndef __MATERIALBASE_FRAGMENT_GLSL__
#define __MATERIALBASE_FRAGMENT_GLSL__

layout(location = 0) out vec4 finalColor;

//--------------------------------------------------
// Function declarations

// To be defined by the shading model
vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection, in float lightIntensity, in vec3 viewDir);

void SetupGlobalVars();
vec4 ComputeFragmentColor();
float GetLightComplexity();

uint ComputeClusterIndex();
uint GetClusterIndex();

uint GetTotalLightCount();
uint GetRelevantLightCount();

uint GetTotalPointLightCount();
uint ComputeRelevantPointLightCount();
uint GetRelevantPointLightCount();
PointLight GetRelevantPointLight(in uint index);


//--------------------------------------------------
// Global vars

vec3 g_viewDir;
vec3 g_normal;
vec3 g_clusterIndex;
uint g_relevantPointlightCount;

void main()
{
  SetupGlobalVars();

#ifdef VRM_LIGHT_COMPLEXITY
  float cplx = GetLightComplexity();
  vec4 shadeColor = vec4(cplx, 0.f, 1.f - cplx, 1.f);
#else
  vec4 shadeColor = ComputeFragmentColor();
#endif

  finalColor = shadeColor;
}

void SetupGlobalVars()
{
  g_viewDir = (u_ViewPosition - v_Position);
  g_normal = normalize(v_Normal);
  g_clusterIndex = ComputeClusterIndex();
  g_relevantPointlightCount = ComputeRelevantPointLightCount();
}

vec4 ComputeFragmentColor()
{
  uint pointLightCount = GetRelevantPointLightCount();
  
  for (int i = 0; i < pointLightCount; ++i)
  {
    PointLight pointLight = GetRelevantPointLight(i);

    vec3 lightPos = vec3(pointLight.position[0], pointLight.position[1], pointLight.position[2]);
    vec3 lightDir = (lightPos - v_Position);

    float lightDistance2 = dot(lightDir, lightDir);
    if (lightDistance2 > pointLight.radius * pointLight.radius)
        continue;

    vec3 lightColor = vec3(pointLight.color[0], pointLight.color[1], pointLight.color[2]);
    float lightIntensity = pointLight.intensity / (4.f * VRM_PI * lightDistance2);

    float lightContribution = ShadingModel(lightColor, lightDir, lightIntensity, g_viewDir);

    shadeColor += lightContribution;
  }

  return vec4(shadeColor, 1.f);
}

float GetLightComplexity()
{
  return float(GetRelevantLightCount()) / float(GetTotalLightCount());
}

uint ComputeClusterIndex()
{
  // Coordinates of the frag in VS for finding the right cluster
  uint zCoord = uint((log(abs(v_CameraDepth) / u_Near) * zCount) / log(u_Far / u_Near));
  vec2 clusterSizeXY = vec2(u_ViewportSize) / vec2(xCount, yCount);

  // Coordinates of the frag in NDC space for finding the right cluster
  uvec3 clusterCoords = ivec3(gl_FragCoord.xy / clusterSizeXY, zCoord);
  uint clusterIndex = clusterCoords.z * (yCount * xCount) + clusterCoords.y * (xCount) + clusterCoords.x;

  return clusterIndex;
}

uint GetClusterIndex()
{
  return g_clusterIndex;
}

uint GetTotalLightCount()
{
  return GetTotalPointLightCount();
}

uint GetRelevantLightCount()
{
  return GetRelevantPointLightCount();
}

uint GetTotalPointLightCount()
{
  return LightBlock_pointLightCount;
}

uint ComputeRelevantPointLightCount()
{
  uint clusterIndex = GetClusterIndex();
  uint lightsCount = clusters[clusterIndex].indexCount;
  return lightsCount;
}

uint GetRelevantPointLightCount()
{
  return g_relevantPointlightCount;
}

PointLight GetRelevantPointLight(in uint index)
{
  uint clusterIndex = GetClusterIndex();
  uint lightIndex = clusters[clusterIndex].lightIndices[index];
  return LightBlock_pointLights[lightIndex];
}

float linearizeDepth(float depth)
{
  return (2.0 * u_Near * u_Far) / (u_Far + u_Near - (depth * 2.0 - 1.0) * (u_Far - u_Near));
}

#endif
