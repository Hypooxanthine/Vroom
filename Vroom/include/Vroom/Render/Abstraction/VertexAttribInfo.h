#pragma once

#include <concepts>
#include <array>

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  struct VertexAttribInfo
  {
    enum EType : uint8_t
    {
      eByte = 0, eUbyte,
      eShort, eUshort,
      eFloat, eInt, eUint,
      eCount,
    };

    inline constexpr VertexAttribInfo(EType type, uint8_t elementCount, bool normalized, size_t offset)
        : type(type), elementCount(elementCount), normalized(normalized), offset(offset)
    {
    }

    EType type;
    uint8_t elementCount; // Ex: vec3 -> elemCount = 3
    bool normalized;
    size_t offset;

    template <EType type>
    inline static consteval GLenum GetGLType()
    {
      constexpr std::array<GLenum, EType::eCount> table =
      {
        GL_BYTE, GL_UNSIGNED_BYTE,
        GL_SHORT, GL_UNSIGNED_SHORT,
        GL_FLOAT, GL_INT, GL_UNSIGNED_INT
      };

      static_assert(type >= 0 && type < EType::eCount, "Unsupported type");
      return table.at(type);
    }
  };

  template <typename T>
  concept CIVertex =
      requires {
        { T::GetElementCount() } -> std::same_as<GLuint>;
      } && requires {
        { T::GetStride() } -> std::same_as<GLuint>;
      } && requires {
        { T::GetElementInfos() } -> std::same_as<std::array<VertexAttribInfo, T::GetElementCount()>>;
      };

} // namespace vrm::gl
