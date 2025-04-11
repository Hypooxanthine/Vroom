#ifndef _COMMONLIGHTS_GLSL_
#define _COMMONLIGHTS_GLSL_

struct DirectionalLight
{
  float direction[3];
  float color[3];
  float intensity;
};

struct PointLight
{
  float position[3];
  float color[3];
  float intensity;
  float radius;
};

#endif
