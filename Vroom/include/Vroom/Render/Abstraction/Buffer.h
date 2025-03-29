#pragma once

#include <span>

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  template <GLenum TARGET>
  class Buffer
  {
  public:
    inline constexpr Buffer() : m_renderID(0), m_capacity(0)
    {
    }

    inline virtual ~Buffer()
    {
      if (m_renderID != 0)
      {
        glDeleteBuffers(1, &m_renderID);
        m_renderID = 0;
        m_capacity = 0;
      }
    }

    inline Buffer(const Buffer &) = delete;
    inline Buffer &operator=(const Buffer &) = delete;

    inline constexpr Buffer(Buffer &&other) noexcept
    {
      m_renderID = std::move(other.m_renderID);
      m_capacity = std::move(other.m_capacity);

      other.m_renderID = 0;
      other.m_capacity = 0;
    }

    inline constexpr Buffer &operator=(Buffer &&other) noexcept
    {
      if (this != &other)
      {
        m_renderID = std::move(other.m_renderID);
        m_capacity = std::move(other.m_capacity);

        other.m_renderID = 0;
        other.m_capacity = 0;
      }

      return *this;
    }

    inline constexpr GLuint getRenderId() const { return m_renderID; }
    inline constexpr GLuint getCapacity() const { return m_capacity; }

    inline void bind() const
    {
      glBindBuffer(TARGET, m_renderID);
    }

    inline void reset(GLsizei capacity, GLenum usage = GL_DYNAMIC_DRAW)
    {
      if (m_renderID == 0)
      {
        glCreateBuffers(1, &m_renderID);
      }

      bind();
      glBufferData(TARGET, capacity, nullptr, usage);
      m_capacity = capacity;
    }

    inline void setData(const void* data, GLsizei dataSize, GLintptr offset = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      if (m_renderID == 0 || dataSize > m_capacity)
      {
        reset(dataSize, usage);
      }
      else
      {
        bind();
      }

      glBufferSubData(TARGET, offset, dataSize, data);
    }

  protected:
  private:
    GLuint m_renderID;
    GLsizei m_capacity;
  };

  template <typename T, GLenum TARGET>
  class ArrayBuffer : public Buffer<TARGET>
  {
  public:

    inline void setData(std::span<const T> data, GLuint offsetElement = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Buffer<TARGET>::setData(data.data(), static_cast<GLsizei>(data.size_bytes()), offsetElement * sizeof(T), usage);
    }
  };

} // namespace vrm::gl
