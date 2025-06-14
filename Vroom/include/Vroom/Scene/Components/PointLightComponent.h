#pragma once

#include <glm/glm.hpp>
#include "Vroom/Api.h"

namespace vrm
{

  struct VRM_API PointLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1000.0f;
    float radius = 50.0f;
  };

} // namespace vrm
