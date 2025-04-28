#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct alignas(sizeof(glm::vec4)) SSBODirectionalLightData
  {
    // glm::mat4 viewProj;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool castsShadows;
  };

} // namespace vrm
