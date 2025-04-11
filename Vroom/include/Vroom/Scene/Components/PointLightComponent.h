#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct PointLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 50.0f;
    float radius = 50.0f;
  };

} // namespace vrm
