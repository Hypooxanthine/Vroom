#ifndef _MATERIALBASE_FRAGMENT_GLSL_
#define _MATERIALBASE_FRAGMENT_GLSL_

layout(location = 0) out vec4 finalColor;

//--------------------------------------------------
// Function declarations

// To be defined by the shading model
vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection);

void SetupGlobalVars();
vec4 ComputeFragmentColor();
float GetLightComplexity();

uint GetTotalLightCount();
uint GetRelevantLightCount();

uint GetTotalDirectionalLightCount();
uint GetRelevantDirectionalLightCount();
DirectionalLight GetRelevantDirectionalLight(in uint index);

uint GetTotalPointLightCount();
uint GetRelevantPointLightCount();
PointLight GetRelevantPointLight(in uint index);


//--------------------------------------------------
// Global vars

vec3 g_viewDir;
vec3 g_normal;

void main()
{
#if defined(VRM_SHADOW_PASS)
  discard;
#endif

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
  g_viewDir = normalize(u_ViewPosition.xyz - v_Position.xyz);
  g_normal = normalize(v_Normal);

#ifdef VRM_CLUSTERED_RENDERING
  SetupGlobalVars_ClusteredRendering();
#endif
}

vec4 ComputeFragmentColor()
{
  vec3 shadeColor = vec3(0.f, 0.f, 0.f);

  uint dirLightCount = GetRelevantDirectionalLightCount();

  for (int i = 0; i < dirLightCount; ++i)
  {
    DirectionalLight dirLight = GetRelevantDirectionalLight(i);
    vec3 lightDir = normalize(vec3(dirLight.direction[0], dirLight.direction[1], dirLight.direction[2]));
    vec3 lightColor = vec3(dirLight.color[0], dirLight.color[1], dirLight.color[2]);

    if (dot(lightDir, g_normal) < 0)
      continue;
    
    lightDir = normalize(lightDir);

    vec3 lightContribution = ShadingModel(lightColor * dirLight.intensity, lightDir);

    shadeColor += lightContribution;
  }

  uint pointLightCount = GetRelevantPointLightCount();
  
  for (int i = 0; i < pointLightCount; ++i)
  {
    PointLight pointLight = GetRelevantPointLight(i);

    vec3 lightPos = vec3(pointLight.position[0], pointLight.position[1], pointLight.position[2]);
    vec3 lightDir = (lightPos - v_Position);

    if (dot(lightDir, g_normal) < 0)
      continue;

    float lightDistance2 = dot(lightDir, lightDir);
    if (lightDistance2 > pointLight.radius * pointLight.radius)
        continue;

    lightDir = normalize(lightDir);
    vec3 lightColor = vec3(pointLight.color[0], pointLight.color[1], pointLight.color[2]);
    float lightIntensity = pointLight.intensity / (4.f * VRM_PI * lightDistance2);

    vec3 lightContribution = ShadingModel(lightColor * lightIntensity, lightDir);

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
  return GetTotalPointLightCount() + GetTotalDirectionalLightCount();
}

uint GetRelevantLightCount()
{
  return GetRelevantPointLightCount() + GetRelevantDirectionalLightCount();
}

uint GetTotalDirectionalLightCount()
{
  return LightBlock_directionalLightCount;
}

uint GetRelevantDirectionalLightCount()
{
  return GetTotalDirectionalLightCount();
}

DirectionalLight GetRelevantDirectionalLight(in uint index)
{
  return LightBlock_directionalLights[index];
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
