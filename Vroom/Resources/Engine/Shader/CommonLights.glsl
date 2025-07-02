#ifndef _COMMONLIGHTS_GLSL_
#define _COMMONLIGHTS_GLSL_

struct DirectionalLight
{
  mat4 viewProj;
  vec4  direction;
  vec4  color;
  float intensity;
  uint  castsShadows;
};

struct PointLight
{
  vec4  position;
  vec4  color;
  float intensity;
  float radius;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};

#endif
