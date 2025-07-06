#pragma once

#include <array>
#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/VertexAttribInfo.h"

namespace vrm
{

  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };

} // namespace vrm