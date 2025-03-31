#pragma once

#include <span>

#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{

  enum class BufferBehaviour : uint8_t
  {
    Static,
    AutoResize
  };

  template <GLenum TARGET, BufferBehaviour Behaviour = BufferBehaviour::Static>
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
        [[likely]]
        glDeleteBuffers(1, &m_renderID);
        m_renderID = 0;
        m_capacity = 0;
      }
    }

    inline Buffer(const Buffer &) = delete;
    inline Buffer &operator=(const Buffer &) = delete;

    inline constexpr Buffer(Buffer &&other) noexcept
      : Buffer()
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
        [[likely]]
        if (m_renderID != 0)
        {
          glDeleteBuffers(1, &m_renderID);
        }

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

    inline static void Unbind()
    {
      glBindBuffer(TARGET, 0);
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

    inline void setData(const void *data, GLsizei dataSize, GLintptr offset = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      GLsizei neededCapacity = dataSize + static_cast<GLsizei>(offset);

      if (m_renderID == 0)
      {
        // For the first setData, we let user create and assign data in the same time
        reset(neededCapacity, usage);
      }
      else if (m_capacity < neededCapacity)
      {

        if constexpr (Behaviour == BufferBehaviour::Static)
        {
          // User can set sub data if he wants,
          // but he has to make sure the buffer is large enough
          // (recreate it, or ensure it is large enough at creation)
          VRM_ASSERT_MSG(false, "Buffer is too small. Please use reset() with a correct capacity first.");
        }
        else // if constexpr (Behaviour == BufferBehaviour::AutoResize)
        {
          GLsizei grownCapacity = neededCapacity + neededCapacity / 2;

          if (offset > 0)
          {
            Buffer intermediate;

            // Final buffer data has to be attached to the same render ID.
            // If we fill another buffer with data and just std::move, 
            // this buffer will have the correct data, but will have another
            // render ID, and then will loose all attached properties,
            // such as SSBO binding point

            intermediate.reset(m_capacity);
            intermediate.copyDataFrom(*this, m_capacity, 0, 0, usage);
            reset(grownCapacity, usage);
            copyDataFrom(intermediate, intermediate.getCapacity(), 0, 0, usage);
          }
          else // If offset is 0, everything will be overriden so no need to copy
          {
            reset(grownCapacity, usage);
          }
        }
      }

      bind();
      glBufferSubData(TARGET, offset, dataSize, data);
    }

    inline void copyDataFrom(const Buffer &other, GLsizei size, GLintptr otherOffset = 0, GLintptr offset = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      VRM_ASSERT_MSG(other.getRenderID() != 0, "Other buffer is not valid");

      if (m_renderID == 0)
      {
        reset(size + static_cast<GLsizei>(offset), usage);
      }
      else
      {
        VRM_ASSERT_MSG(size + offset <= m_capacity, "Destination buffer is too small. Please use reset() with a correct capacity first.");
        bind();
      }

      glBindBuffer(GL_COPY_READ_BUFFER, other.getRenderID());
      glBindBuffer(GL_COPY_WRITE_BUFFER, getRenderID());
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, otherOffset, offset, size);
    }

  protected:
  private:
    GLuint m_renderID;
    GLsizei m_capacity;
  };

  template <typename T, GLenum TARGET, BufferBehaviour Behaviour = BufferBehaviour::Static>
  class ArrayBuffer : public Buffer<TARGET, Behaviour>
  {
  public:
    inline void setData(std::span<const T> data, GLuint offsetElement = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Buffer<TARGET, Behaviour>::setData(data.data(), static_cast<GLsizei>(data.size_bytes()), offsetElement * sizeof(T), usage);
    }
  };

} // namespace vrm::gl
