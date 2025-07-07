#ifndef _SHADINGMODEL_MICROFACETS_IMPL_GLSL_
#define _SHADINGMODEL_MICROFACETS_IMPL_GLSL_

vec3 GetAlbedo()
{
#ifdef VRM_Microfacets_Albedo_UNIFORM
  return u_MicrofacetsAlbedo;
#elif defined (VRM_Microfacets_Albedo_TEXTURE) && defined (VRM_Microfacets_Albedo_TEXTURE_SLOT)
  return texture(u_MicrofacetsTextures[VRM_Microfacets_Albedo_TEXTURE_SLOT], v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

float GetSpecular()
{
#ifdef VRM_Microfacets_Specular_UNIFORM
  return u_MicrofacetsSpecular;
#elif defined (VRM_Microfacets_Specular_TEXTURE) && defined (VRM_Microfacets_Specular_TEXTURE_SLOT)
  return texture(u_MicrofacetsTextures[VRM_Microfacets_Specular_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetRoughness()
{
#ifdef VRM_Microfacets_Roughness_UNIFORM
  return u_MicrofacetsRoughness;
#elif defined (VRM_Microfacets_Roughness_TEXTURE) && defined (VRM_Microfacets_Roughness_TEXTURE_SLOT)
  return texture(u_MicrofacetsTextures[VRM_Microfacets_Roughness_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetMetalness()
{
#ifdef VRM_Microfacets_Metalness_UNIFORM
  return u_MicrofacetsMetalness;
#elif defined (VRM_Microfacets_Metalness_TEXTURE) && defined (VRM_Microfacets_Metalness_TEXTURE_SLOT)
  return texture(u_MicrofacetsTextures[VRM_Microfacets_Metalness_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.0;
#endif
}

vec3 g_albedo;
float g_specular;
float g_metallness;
float g_roughness;
vec3 g_F0;

float GeometryGGX(float NdotV)
{
  float r = g_roughness + 1.0;
  float k = (r * r) / 8.0;

  return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 lightDir)
{
  float NdotV = max(dot(g_normal, g_viewDir), 0.0);
  float NdotL = max(dot(g_normal, lightDir), 0.0);

  return GeometryGGX(NdotV) * GeometryGGX(NdotL);
}


float NormalDistribution(in vec3 bisector)
{
  float alpha = g_roughness * g_roughness;
  float nDotBisector = dot(g_normal, bisector);
  float denom = (nDotBisector * nDotBisector * (alpha * alpha - 1.0) + 1.0);
  denom = VRM_PI * denom * denom;

  return alpha * alpha / denom;
}

vec3 Fresnel(in float cosTheta, in vec3 F0)
{
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fCookTorrance(in vec3 viewDir, in vec3 lightDir, in vec3 F, in vec3 bisector)
{
  float cosTheta = dot(g_normal, viewDir);

  float NDF = NormalDistribution(bisector);
  float G = GeometrySmith(lightDir);

  return NDF * G * F / (4.0 * max(0.0, dot(g_normal, viewDir)) * max(0.0, dot(g_normal, lightDir)) + 0.0001);
}

float fLambert()
{
  return 1.0 / VRM_PI;
}

vec3 BRDF(in vec3 viewDir, in vec3 lightDir)
{
  vec3 bisector = normalize(viewDir + lightDir);
  vec3 fresnel = Fresnel(max(dot(bisector, viewDir), 0.0), g_F0);
  vec3 ks = vec3(fresnel);
  vec3 kd = (vec3(1.0) - ks) * (1.0 - g_metallness);

  return kd * fLambert() * g_albedo + ks * fCookTorrance(viewDir, lightDir, fresnel, bisector);
}

void SetupGlobalVars_ShadingModel()
{
  g_albedo = GetAlbedo();
  g_specular = GetSpecular();
  g_metallness = GetMetalness();
  g_roughness = GetRoughness();
  g_F0 = mix(vec3(0.04), g_albedo, g_metallness);
}

vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection)
{
  return BRDF(g_viewDir, lightDirection) * lightColor * max(0.0, dot(g_normal, lightDirection));
}

#endif // _SHADINGMODEL_MICROFACETS_IMPL_GLSL_
