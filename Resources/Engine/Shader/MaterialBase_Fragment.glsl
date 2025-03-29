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

uint GetTotalLightCount();
uint GetRelevantLightCount();

uint GetTotalPointLightCount();
uint GetRelevantPointLightCount();
PointLight GetRelevantPointLight(in uint index);


//--------------------------------------------------
// Global vars

vec3 g_viewDir;
vec3 g_normal;

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

#ifdef VRM_CLUSTERED_RENDERING
  SetupGlobalVars_ClusteredRendering();
#endif
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

uint GetRelevantPointLightCount()
{
#ifdef VRM_CLUSTERED_RENDERING
  return GetRelevantPointLightCount_ClusteredRendering();
#else
  return GetTotalPointLightCount();
#endif
}

PointLight GetRelevantPointLight(in uint index)
{
#ifdef VRM_CLUSTERED_RENDERING
  return GetRelevantPointLight_ClusteredRendering(index);
#else
  return LightBlock_pointLights[index];
#endif
}

float linearizeDepth(float depth)
{
  return (2.0 * u_Near * u_Far) / (u_Far + u_Near - (depth * 2.0 - 1.0) * (u_Far - u_Near));
}

#endif
