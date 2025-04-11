#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct DirectionalLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 0.5f;
  };

} // namespace vrm
