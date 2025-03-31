#pragma once

#include <unordered_set>

#include "Vroom/Render/Abstraction/VertexAttribInfo.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"

namespace vrm::gl
{

  class VertexArray
  {
  public:
    inline VertexArray() : m_renderID(0)
    {
    }

    inline virtual ~VertexArray()
    {
      destroy();
    }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    inline VertexArray(VertexArray &&other) noexcept
      : VertexArray()
    {
      m_renderID = std::move(other.m_renderID);
      other.m_renderID = 0;
    }

    inline VertexArray &operator=(VertexArray &&other) noexcept
    {
      if (this != &other)
      {
        destroy();

        m_renderID = std::move(other.m_renderID);
        other.m_renderID = 0;
      }

      return *this;
    }

    inline bool isValid() const { return m_renderID != 0; }

    inline void bind() const { glBindVertexArray(m_renderID); }

    inline void create()
    {
      destroy();
      glGenVertexArrays(1, &m_renderID);
    }

    inline void destroy()
    {
      if (m_renderID != 0)
      {
        glDeleteVertexArrays(1, &m_renderID);
        m_renderID = 0;
      }
    }

    inline bool isBindingUsed(GLuint binding) const { return m_bindings.contains(binding); }

    template <uint8_t VertexElementIndex, typename V>
      requires CIVertex<V>
    inline void defineAttribute(const VertexBuffer<V>& vb, GLuint binding = VertexElementIndex)
    {
      static_assert(VertexElementIndex < V::GetElementCount(), "Requested Vertex class attribute is too big");
      constexpr VertexAttribInfo info = V::GetElementInfos().at(VertexElementIndex);

      defineAttribute(vb, binding, VertexAttribInfo::GetGLType<info.type>(), info.elementCount, info.normalized, V::GetStride(), info.offset);
    }

    template <typename V>
    inline void defineAttribute(const VertexBuffer<V>& vb, GLuint binding, GLenum glType, GLint elemCount, bool normalized, GLsizei stride, size_t offset)
    {
      VRM_ASSERT_MSG(isValid(), "You must create the vao first");
      VRM_ASSERT_MSG(!isBindingUsed(binding), "Binding already used");

      bind();
      vb.bind();

      glEnableVertexAttribArray(binding);
      m_bindings.emplace(binding);

      glVertexAttribPointer(
        binding,
        elemCount,
        glType,
        normalized ? GL_TRUE : GL_FALSE,
        stride,
        reinterpret_cast<const void*>(static_cast<uintptr_t>(offset))
      );

      vb.Unbind();
    }

  private:
    GLuint m_renderID;
    std::unordered_set<GLuint> m_bindings;
  };

} // namespace vrm::gl