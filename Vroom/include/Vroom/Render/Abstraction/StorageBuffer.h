#pragma once

#include "Vroom/Render/Abstraction/BindingPointAllocator.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::gl
{

  template <BufferBehaviour Behaviour>
  class StorageBufferBase : public Buffer<GL_SHADER_STORAGE_BUFFER, Behaviour>
  {
  public:
    using Parent = Buffer<GL_SHADER_STORAGE_BUFFER, Behaviour>;

  public:

    inline constexpr StorageBufferBase() : Parent()
    {
    }

    inline virtual ~StorageBufferBase()
    {
      if (isBindingPointValid())
      {
        s_allocator.giveBack(m_bindingPoint);
        m_bindingPoint = s_allocator.Invalid;
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

    inline void reset(GLsizei capacity, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Parent::reset(capacity, usage);
      // If no binding point has been given, we generate one and bind this ssbo to it
      // If one was already given, we let the ssbo on its binding point
      if (!isBindingPointValid())
      {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, s_allocator.next(), this->getRenderID());
      }
    }

  private:
    inline static BindingPointAllocator<GLuint> s_allocator = { 0, 64 };

    GLuint m_bindingPoint = s_allocator.Invalid;
  };

  using StorageBuffer = StorageBufferBase<BufferBehaviour::Static>;

  using AutoResizeStorageBuffer = StorageBufferBase<BufferBehaviour::AutoResize>;

} // namespace vrm::gl
