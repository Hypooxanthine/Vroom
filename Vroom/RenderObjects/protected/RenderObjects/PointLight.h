#pragma once

#include <glm/glm.hpp>

namespace vrm::render
{

struct PointLight
{
  glm::vec3 position;
  glm::vec3 color;
  float intensity;
  float radius;
  float smoothRadius;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};
  
}