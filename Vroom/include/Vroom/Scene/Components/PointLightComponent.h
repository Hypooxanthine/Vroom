#pragma once

#include <glm/glm.hpp>
#include "Vroom/Api.h"

namespace vrm
{

  struct VRM_API PointLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 5.0f;
    float radius = 30.0f;
    float constantAttenuation = 1.0f;
    float linearAttenuation = 0.0f;
    float quadraticAttenuation = 0.1f;
  };

} // namespace vrm
