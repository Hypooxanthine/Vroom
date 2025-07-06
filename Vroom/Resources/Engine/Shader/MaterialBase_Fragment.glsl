#ifndef _MATERIALBASE_FRAGMENT_GLSL_
#define _MATERIALBASE_FRAGMENT_GLSL_

#if defined(VRM_ENTITY_PICKING)
  layout(location = 0) out uint finalColor;
#else
  layout(location = 0) out vec4 finalColor;
#endif

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

float ComputeDirectionalShadowFactor(in uint shadowCasterId, in float lightDotNormal);


//--------------------------------------------------
// Global vars

vec3 g_viewDir;
vec3 g_normal;
ivec2 g_fragCoords;

void main()
{
#if defined(VRM_SHADOW_PASS)
  return;
#endif

#if defined(VRM_ENTITY_PICKING)
  finalColor = u_EntityId;
  return;
#else

  SetupGlobalVars();

#ifdef VRM_LIGHT_COMPLEXITY
  float cplx = GetLightComplexity();
  vec4 shadeColor = vec4(cplx, 0.f, 1.f - cplx, 1.f);
#else
  vec4 shadeColor = ComputeFragmentColor();
#endif

  finalColor = shadeColor;
#endif
}

void SetupGlobalVars()
{
  g_viewDir = normalize(u_ViewPosition.xyz - v_Position.xyz);
  g_normal = normalize(v_Normal);

  g_fragCoords = ivec2(floor(gl_FragCoord.xy));

#ifdef VRM_CLUSTERED_RENDERING
  SetupGlobalVars_ClusteredRendering();
#endif
}

vec4 ComputeFragmentColor()
{
  vec3 shadeColor = vec3(0.f, 0.f, 0.f);

  uint dirLightCount = GetRelevantDirectionalLightCount();
  uint shadowCasterId = 0;

  for (int i = 0; i < dirLightCount; ++i)
  {
    DirectionalLight dirLight = GetRelevantDirectionalLight(i);
    vec3 lightDir = normalize(dirLight.direction.xyz);
    float lightDotNormal = dot(lightDir, g_normal);

    float lightVisibility = 1.f;

    if (lightDotNormal > 0 && dirLight.castsShadows > 0)
    {
      lightVisibility = (1.f - ComputeDirectionalShadowFactor(shadowCasterId, lightDotNormal));
      ++shadowCasterId;
    }

    
    lightDir = normalize(lightDir);

    vec3 lightContribution = ShadingModel(dirLight.color.xyz * dirLight.intensity * lightVisibility, lightDir);

    shadeColor += lightContribution;
  }

  uint pointLightCount = GetRelevantPointLightCount();
  
  for (int i = 0; i < pointLightCount; ++i)
  {
    PointLight pointLight = GetRelevantPointLight(i);

    vec3 lightDir = (pointLight.position.xyz - v_Position);

    float lightDistance2 = dot(lightDir, lightDir);

    if (lightDistance2 < 0)
      continue;
      
    if (lightDistance2 > pointLight.radius * pointLight.radius)
        continue;

    float lightDistance = sqrt(lightDistance2);

    lightDir = normalize(lightDir);
    vec3 lightColor = vec3(pointLight.color[0], pointLight.color[1], pointLight.color[2]);
    float lightIntensity =
      pointLight.intensity / (
        pointLight.constantAttenuation
      + pointLight.linearAttenuation    * lightDistance
      + pointLight.quadraticAttenuation * lightDistance2
      );

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
  return g_directionalLightCount;
}

uint GetRelevantDirectionalLightCount()
{
  return GetTotalDirectionalLightCount();
}

DirectionalLight GetRelevantDirectionalLight(in uint index)
{
  return g_directionalLights[index];
}

uint GetTotalPointLightCount()
{
  return g_pointLightCount;
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
  return g_pointLights[index];
#endif
}

float linearizeDepth(float depth)
{
  return (2.0 * u_Near * u_Far) / (u_Far + u_Near - (depth * 2.0 - 1.0) * (u_Far - u_Near));
}

float ComputeDirectionalShadowFactor(in uint shadowCasterId, in float lightDotNormal)
{
  const mat4 lightMatrix = g_directionalLightMatrices[shadowCasterId];
  const vec4 pos_clipspace_light = lightMatrix * vec4(v_Position, 1.f);
  const vec4 pos_ndc_light = pos_clipspace_light / pos_clipspace_light.w;

  if(pos_ndc_light.z >= 1.0)
    return 0.f;
  
  const vec3 texCoords = vec3
  (
    pos_ndc_light.x / 2.f + 0.5f,
    pos_ndc_light.y / 2.f + 0.5f,
    float(shadowCasterId)
  );

  const float fragDepth = pos_ndc_light.z / 2.f + 0.5f;
  const float bias = 0.f;//mix(0.001f, 0.f, lightDotNormal);
  const vec2 pxSize = 1.f / textureSize(u_DirectionalShadowMaps, 0).xy;

  const int kernelRadius = int(u_SoftShadowKernelRadius);

  if (kernelRadius == 0)
  {
    const float lightDepth = texture(u_DirectionalShadowMaps, texCoords, 0.f).x;

    if (fragDepth > lightDepth + bias) // Fragment is further than the closest fragment recorded on this px
    {
      // Frag is in the shadow of light i
      return 1.f;
    }
    else
    {
      return 0.f;
    }
  }

  const int kernelSize = kernelRadius * 2 + 1;
  const int kernelArea = kernelSize * kernelSize;
  float shadowFactor = 0.f;

  for (int i = -kernelRadius; i <= kernelRadius; ++i)
  {
    for (int j = -kernelRadius; j <= kernelRadius; ++j)
    {
      const float lightDepth = texture(u_DirectionalShadowMaps, texCoords + vec3(vec2(float(i), float(j)) * pxSize, 0.f), 0.f).x;

      if (fragDepth > lightDepth + bias)
      {
        shadowFactor += 1.f;
      }
    }
  }
  
  return shadowFactor / float(kernelArea);
}

#endif
