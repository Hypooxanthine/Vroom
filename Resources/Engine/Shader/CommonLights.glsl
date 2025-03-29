#ifndef _COMMONLIGHTS_GLSL_
#define _COMMONLIGHTS_GLSL_

struct PointLight
{
  float position[3];
  float color[3];
  float intensity;
  float radius;
};

#endif
