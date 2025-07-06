#pragma once

#include <cstdint>
#include <array>

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  class Buffer;

  class VertexArray
  {
  public:

    inline static constexpr size_t s_maxAttribCount = 16;

    struct Attribute
    {
      GLenum componentType = GL_FLOAT; // Float, int, uint, ...
      uint32_t componentCount = 4; // vec1, vec2, vec3 or vec4 (1, 2, 3 or 4)
      bool normalized;
      uint32_t vertexBufferStrideBytes = 0; // 0 for tightly packed
      uint32_t vertexBufferOffsetBytes = 0;
    };

  public:

    VertexArray();
    ~VertexArray() noexcept;

    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(const VertexArray&) = delete;

    VertexArray& operator=(VertexArray&& other);
    VertexArray(VertexArray&& other);

    void create();
    void release() noexcept;

    void enableAttribute(uint32_t attribIndex, const Attribute& attrib, const Buffer& vertexBuffer);
    void disableAttribute(uint32_t attribIndex);
    inline bool isAttributeIndexEnabled(uint32_t attribIndex) const { return m_attribs.at(attribIndex).enabled; }

    static void Bind(GLuint renderId);
    inline static void Bind(const VertexArray& vertexArray) { Bind(vertexArray.getRenderId()); }
    inline static void Unbind() { Bind(0); }

    inline GLuint getRenderId() const { return m_renderId; }

  private:

    struct AttribState
    {
      bool enabled = false;
      Attribute attrib = {};
    };

  private:

    GLuint m_renderId = 0;
    std::array<AttribState, s_maxAttribCount> m_attribs = {};

  };

} // namespace vrm::gl