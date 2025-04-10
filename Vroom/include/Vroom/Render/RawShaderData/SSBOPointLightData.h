#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct SSBOPointLightData
  {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
  };

} // namespace vrm
