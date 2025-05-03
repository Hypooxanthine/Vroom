#pragma once

#include <glm/glm.hpp>

namespace vrm
{

  struct alignas(sizeof(glm::vec4)) SSBODirectionalLightData
  {
    static constexpr size_t Alignment = sizeof(glm::vec4);

    glm::mat4 viewProj;
    glm::vec4 direction;
    glm::vec4 color;
    float intensity;
    uint32_t castsShadows;
    float _pad0[2];
  };

} // namespace vrm
