#pragma once

#include <glm/glm.hpp>
#include "Vroom/Api.h"

namespace vrm
{

  struct VRM_API DirectionalLightComponent
  {
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 0.5f;
    bool castsShadows = true;
  };

} // namespace vrm
