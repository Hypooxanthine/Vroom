#ifndef _COMMONLIGHTS_GLSL_
#define _COMMONLIGHTS_GLSL_

struct DirectionalLight
{
  // mat4 viewProj;
  float direction[3];
  float color[3];
  float intensity;
  bool castsShadows;
};

struct PointLight
{
  float position[3];
  float color[3];
  float intensity;
  float radius;
};

#endif
