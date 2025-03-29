#ifndef __COMMONLIGHTS_GLSL__
#define __COMMONLIGHTS_GLSL__

struct PointLight
{
  float position[3];
  float color[3];
  float intensity;
  float radius;
};

#endif
