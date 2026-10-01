#pragma once

#include <glm/glm.hpp>

namespace vrm::render
{

struct DirectionalLight
{
  glm::vec3 direction;
  glm::vec3 color;
  float intensity;
  bool castsShadows;
};

} // namespace vrm::render
