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
        this->~Buffer();

        m_renderID = std::move(other.m_renderID);
        m_capacity = std::move(other.m_capacity);

        other.m_renderID = 0;
        other.m_capacity = 0;
      }

      return *this;
    }

    inline constexpr GLuint getRenderID() const { return m_renderID; }
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
      if (m_renderID == 0)
      {
        // For the first setData, we let user create and assign data in the same time
        reset(dataSize + static_cast<GLuint>(offset), usage);
      }
      else
      {
        // After that, user can set sub data if he wants,
        // but he has to make sure the buffer is large enough
        // (recreate it, or ensure it is large enough at creation)
        VRM_ASSERT_MSG(dataSize + offset <= m_capacity, "Buffer is too small. Please use reset() with a correct capacity first. In last resort, use a DynamicStorageBuffer");
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
