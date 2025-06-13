#ifndef _SHADINGMODEL_PHONG_IMPL_GLSL_
#define _SHADINGMODEL_PHONG_IMPL_GLSL_

vec3 GetDiffuse()
{
#ifdef VRM_Phong_Diffuse_UNIFORM
  return u_PhongDiffuse;
#elif defined (VRM_Phong_Diffuse_TEXTURE) && defined (VRM_Phong_Diffuse_TEXTURE_SLOT)
  return texture(u_PhongTextures[VRM_Phong_Diffuse_TEXTURE_SLOT], v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

vec3 GetSpecular()
{
#ifdef VRM_Phong_Specular_UNIFORM
  return u_PhongSpecular;
#elif defined (VRM_Phong_Specular_TEXTURE) && defined (VRM_Phong_Specular_TEXTURE_SLOT)
  return texture(u_PhongTextures[VRM_Phong_Specular_TEXTURE_SLOT], v_TexCoord).xyz;
#else
  return vec3(1.f, 1.f, 1.f);
#endif
}

float GetShininess()
{
#ifdef VRM_Phong_Shininess_UNIFORM
  return u_PhongShininess;
#elif defined (VRM_Phong_Shininess_TEXTURE) && defined (VRM_Phong_Shininess_TEXTURE_SLOT)
  return texture(u_PhongTextures[VRM_Phong_Shininess_TEXTURE_SLOT], v_TexCoord).x;
#else
  return 1.f;
#endif
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
