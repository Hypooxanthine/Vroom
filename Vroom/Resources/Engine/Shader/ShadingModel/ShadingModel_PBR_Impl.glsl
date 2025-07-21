#ifndef _SHADINGMODEL_PBR_IMPL_GLSL_
#define _SHADINGMODEL_PBR_IMPL_GLSL_

vec3 GetAlbedo()
{
#ifdef VRM_UNIFORM_u_albedo
  return u_albedo;
#elif defined (VRM_TEXTURE_u_albedo)
  return texture(u_albedo, v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

float GetSpecular()
{
#ifdef VRM_UNIFORM_u_specular
  return u_specular;
#elif defined (VRM_TEXTURE_u_specular)
  return texture(u_specular, v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetRoughness()
{
#ifdef VRM_UNIFORM_u_roughness
  return u_roughness;
#elif defined (VRM_TEXTURE_u_roughness)
  return texture(u_roughness, v_TexCoord).x;
#else
  return 0.5;
#endif
}

float GetMetalness()
{
#ifdef VRM_UNIFORM_u_metalness
  return u_metalness;
#elif defined (VRM_TEXTURE_u_metalness)
  return texture(u_metalness, v_TexCoord).x;
#else
  return 0.0;
#endif
}

float GetIOR()
{
#ifdef VRM_UNIFORM_u_IOR
  return u_IOR;
#elif defined (VRM_TEXTURE_u_IOR)
  return texture(u_IOR, v_TexCoord).x;
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
  
  if (NdotH <= 0.0) return 0.0;
  
  float NdotH2 = NdotH * NdotH;
  float tanAbs = length(cross(h, g_normal)) / max(NdotH, 0.001);
  float tan2 = tanAbs * tanAbs;

  float num = alpha2 * Xpositive(NdotH);
  float denom = NdotH2 * (alpha2 + tan2);
  denom = VRM_PI * denom * denom;

  return num / max(denom, 0.001);
}

float G1(in vec3 x, in vec3 h, in float alpha2)
{
  float NdotX = dot(g_normal, x);
  
  if (NdotX <= 0.0) return 0.0;
  
  float tanAbs = length(cross(g_normal, x)) / max(NdotX, 0.001);
  float tan2 = tanAbs * tanAbs;

  float nom = Xpositive(dot(x, h) / max(dot(x, g_normal), 0.001)) * 2.0;
  float denom = 1.0 + sqrt(1.0 + alpha2 * tan2);

  return nom / max(denom, 0.001);
}

float Geometric_GGX(in vec3 lightDirection, in vec3 h, in float alpha2)
{
  return G1(g_viewDir, h, alpha2) * G1(lightDirection, h, alpha2);
}

vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection)
{
  float NdotL = dot(g_normal, lightDirection);
  float NdotV = dot(g_normal, g_viewDir);
  
  if (NdotL <= 0.0 || NdotV <= 0.0) return vec3(0.0);
  
  vec3 h = normalize(lightDirection + g_viewDir);
  float VdotH = dot(g_viewDir, h);
  float NdotH = dot(g_normal, h);
  
  if (VdotH <= 0.0 || NdotH <= 0.0) return vec3(0.0);
  
  float alpha = max(g_roughness * g_roughness, 0.001);
  float alpha2 = alpha * alpha;

  vec3 F0 = mix(vec3(g_F0), g_albedo, g_metallness);
  vec3 F = F0 + (1.0 - F0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);

  float D = NormalDistribution_GGX(lightDirection, h, alpha2);
  float G = Geometric_GGX(lightDirection, h, alpha2);

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= (1.0 - g_metallness);

  // Diffuse BRDF (Lambert)
  vec3 diffuse = kD * g_albedo / VRM_PI;

  vec3 numerator = D * G * F;
  float denominator = 4.0 * NdotV * NdotL + 0.001;
  vec3 specular = numerator / denominator;
  
  specular = min(specular, vec3(10.0));

  vec3 result = lightColor * NdotL * (diffuse + specular);
  
  return clamp(result, vec3(0.0), vec3(100.0));
}

#endif // _SHADINGMODEL_PBR_IMPL_GLSL_
