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

    inline static consteval GLuint GetElementCount() { return 3; }

    inline static consteval GLuint GetStride() { return sizeof(Vertex); }

    inline static consteval auto GetElementInfos()
    {
      using Type = gl::VertexAttribInfo::EType;
      return std::array<gl::VertexAttribInfo, 3>
      {
        gl::VertexAttribInfo { Type::eFloat, 3, false, offsetof(Vertex, position) },
        gl::VertexAttribInfo { Type::eFloat, 3, false, offsetof(Vertex, normal) },
        gl::VertexAttribInfo { Type::eFloat, 2, false, offsetof(Vertex, texCoords) }
      };
    }
  };

} // namespace vrm