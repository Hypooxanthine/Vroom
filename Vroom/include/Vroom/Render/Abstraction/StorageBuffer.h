#pragma once

#include "Vroom/Render/Abstraction/BindingPointAllocator.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::gl
{

  class StorageBuffer : public Buffer<GL_SHADER_STORAGE_BUFFER>
  {
  public:
    using Parent = Buffer<GL_SHADER_STORAGE_BUFFER>;

  public:
    inline constexpr StorageBuffer() : Parent()
    {
    }

    inline virtual ~StorageBuffer()
    {
      if (isBindingPointValid())
      {
        s_allocator.giveBack(m_bindingPoint);
        m_bindingPoint = s_allocator.Invalid;
      }
    }

    inline void reset(GLsizei capacity, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Buffer<GL_SHADER_STORAGE_BUFFER>::reset(capacity, usage);
      // If no binding point has been given, we generate one and bind this ssbo to it
      // If one was already given, we let the ssbo on its binding point
      if (!isBindingPointValid())
      {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, s_allocator.next(), getRenderID());
      }
    }

    inline constexpr bool isBindingPointValid() const
    {
      return m_bindingPoint != s_allocator.Invalid;
    }

    inline constexpr GLuint getBindingPoint() const
    {
      return m_bindingPoint;
    }

  private:
    inline static BindingPointAllocator<GLuint> s_allocator = { 0, 64 };

    GLuint m_bindingPoint = s_allocator.Invalid;
  };

} // namespace vrm::gl
