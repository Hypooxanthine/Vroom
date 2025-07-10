#ifndef _SHADINGMODEL_PHONG_IMPL_GLSL_
#define _SHADINGMODEL_PHONG_IMPL_GLSL_

vec3 GetDiffuse()
{
#ifdef VRM_UNIFORM_u_diffuse
  return u_diffuse;
#elif defined (VRM_TEXTURE_u_diffuse)
  return texture(u_diffuse, v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

vec3 GetSpecular()
{
#ifdef VRM_UNIFORM_u_specular
  return u_specular;
#elif defined (VRM_TEXTURE_u_specular)
  return texture(u_specular, v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

float GetShininess()
{
#ifdef VRM_UNIFORM_u_shininess
  return u_shininess;
#elif defined (VRM_TEXTURE_u_shininess)
  return texture(u_shininess, v_TexCoord).x;
#else
  return 1.f;
#endif
}

void SetupGlobalVars_ShadingModel()
{

}

vec3 ShadingModel(in vec3 lightColor, in vec3 lightDirection)
{
  float diff = max(dot(g_normal, lightDirection), 0.f);
  vec3 reflectDir = reflect(-lightDirection, g_normal);
  float spec = pow(max(dot(g_viewDir, reflectDir), 0.f), GetShininess());

  // return diff * GetDiffuse() * lightColor;
  return (diff * GetDiffuse() + spec * GetSpecular()) * lightColor;
}

#endif
