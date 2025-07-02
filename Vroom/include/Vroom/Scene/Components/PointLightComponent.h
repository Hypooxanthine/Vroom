#pragma once

#include <glm/glm.hpp>
#include "Vroom/Api.h"

namespace vrm
{

  struct VRM_API PointLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 100.0f;
    float radius = 50.0f;
    float constantAttenuation = 1.0f;
    float linearAttenuation = 0.09f;
    float quadraticAttenuation = 0.032f;
  };

} // namespace vrm
