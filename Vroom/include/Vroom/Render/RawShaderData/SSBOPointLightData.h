#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct alignas(sizeof(glm::vec4)) SSBOPointLightData
  {
    static constexpr size_t Alignment = sizeof(glm::vec4);

    glm::vec4 position;
    glm::vec4 color;
    float intensity;
    float radius;
    float _pad0[2];
  };

} // namespace vrm
