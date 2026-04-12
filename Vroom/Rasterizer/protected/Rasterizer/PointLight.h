#pragma once

#include <glm/glm.hpp>

namespace vrm::render
{

struct PointLight
{
  glm::vec4 position;
  glm::vec4 color;
  float intensity;
  float radius;
  float smoothRadius;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};
  
}