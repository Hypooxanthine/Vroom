#ifndef _SHADINGMODEL_PBR_IMPL_GLSL_
#define _SHADINGMODEL_PBR_IMPL_GLSL_

vec3 GetAlbedo()
{
#ifdef VRM_PBR_Albedo_UNIFORM
  return u_PBRAlbedo;
#elif defined (VRM_PBR_Albedo_TEXTURE) && defined (VRM_PBR_Albedo_TEXTURE_SLOT)
  return texture(u_PBRTextures[VRM_PBR_Albedo_TEXTURE_SLOT], v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

float GetSpecular()
{
#ifdef VRM_PBR_Specular_UNIFORM
  return u_PBRSpecular;
#elif defined (VRM_PBR_Specular_TEXTURE) && defined (VRM_PBR_Specular_TEXTURE_SLOT)
  return texture(u_PBRTextures[VRM_PBR_Specular_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetRoughness()
{
#ifdef VRM_PBR_Roughness_UNIFORM
  return u_PBRRoughness;
#elif defined (VRM_PBR_Roughness_TEXTURE) && defined (VRM_PBR_Roughness_TEXTURE_SLOT)
  return texture(u_PBRTextures[VRM_PBR_Roughness_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetMetalness()
{
#ifdef VRM_PBR_Metalness_UNIFORM
  return u_PBRMetalness;
#elif defined (VRM_PBR_Metalness_TEXTURE) && defined (VRM_PBR_Metalness_TEXTURE_SLOT)
  return texture(u_PBRTextures[VRM_PBR_Metalness_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 0.0;
#endif
}

float GetIOR()
{
#ifdef VRM_PBR_IOR_UNIFORM
  return u_PBRIOR;
#elif defined (VRM_PBR_IOR_TEXTURE) && defined (VRM_PBR_IOR_TEXTURE_SLOT)
  return texture(u_PBRTextures[VRM_PBR_IOR_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 1.5;
#endif
}

vec3 g_albedo;
float g_specular;
float g_roughness;
float g_metallness;
float g_IOR;
float g_F0;

void SetupGlobalVars_ShadingModel()
{
  g_albedo = GetAlbedo();
  g_specular = GetSpecular();
  g_roughness = GetRoughness();
  g_metallness = GetMetalness();
  g_IOR = GetIOR();
  float IORMinusOne = g_IOR - 1.0;
  float IORPlusOne = g_IOR + 1.0;
  g_F0 = IORMinusOne * IORMinusOne / (IORPlusOne * IORPlusOne);
}

float Xpositive(float value)
{
  if (value > 0.0) return 1.0;
  else return 0.0;
}

float NormalDistribution_GGX(in vec3 lightDirection, in vec3 h, in float alpha2)
{
  float NdotH = dot(g_normal, h);
  float NdotH2 = NdotH * NdotH;
  float tanAbs = length(cross(h, g_normal)) / NdotH;
  float tan2 = tanAbs * tanAbs;

  float num = alpha2 * Xpositive(NdotH);
  float denom = NdotH2 * (alpha2 + tan2);
  denom = VRM_PI * denom * denom;

  return num / denom;
}

float G1(in vec3 x, in vec3 h, in float alpha2)
{
  float NdotX = dot(g_normal, x);
  float tanAbs = length(cross(g_normal, x)) / NdotX;
  float tan2 = tanAbs * tanAbs;

  float nom = Xpositive(dot(x, h) / dot(x, g_normal)) * 2.0;
  float denom = 1.0 + sqrt(1.0 + alpha2 * tan2);

  return nom / denom;
}

float Geometric_GGX(in vec3 lightDirection, in vec3 h, in float alpha2)
{
  return G1(g_viewDir, h, alpha2) * G1(lightDirection, h, alpha2);
}

vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection)
{
  float NdotL = dot(g_normal, lightDirection);
  float NdotV = dot(g_normal, g_viewDir);
  
  vec3 h = normalize(lightDirection + g_viewDir); // Bisector
  float VdotH = dot(g_viewDir, h);
  
  float alpha = g_roughness * g_roughness;
  float alpha2 = alpha * alpha;

  // Calcul du terme de Fresnel (F)
  vec3 F0 = mix(vec3(g_F0), g_albedo, g_metallness);
  vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0); // Fresnel-Schlick

  float D = NormalDistribution_GGX(lightDirection, h, alpha2);
  float G = Geometric_GGX(lightDirection, h, alpha2);

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= (1.0 - g_metallness);

  // Diffuse BRDF (Lambert)
  vec3 diffuse = kD * g_albedo / VRM_PI;

  // Specular BRDF (Cook-Torrance)
  vec3 numerator = D * G * F;
  float denominator = 4.0 * max(NdotV, 0.0) * max(NdotL, 0.0) + 0.0001;
  vec3 specular = numerator / denominator;

  return lightColor * max(NdotL, 0.0) * (diffuse + specular);
}

#endif // _SHADINGMODEL_PBR_IMPL_GLSL_
