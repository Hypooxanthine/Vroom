#pragma once

#include "Vroom/Render/Abstraction/BindingPointAllocator.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::gl
{

  class StorageBufferBase
  {
  public:

    inline constexpr StorageBufferBase()
    {
    }

    inline constexpr StorageBufferBase(StorageBufferBase&& other) noexcept
    {
      *this = std::move(other);
    }

    inline virtual ~StorageBufferBase()
    {
      if (isBindingPointValid())
      {
        s_allocator.giveBack(m_bindingPoint);
        m_bindingPoint = s_allocator.Invalid;
      }
    }

    inline constexpr StorageBufferBase &operator=(StorageBufferBase &&other) noexcept
    {
      if (this != &other)
      {
        if (isBindingPointValid())
        {
          s_allocator.giveBack(m_bindingPoint);
        }
        m_bindingPoint = std::move(other.m_bindingPoint);
        other.m_bindingPoint = s_allocator.Invalid;
      }

      return *this;
    }

    inline constexpr bool isBindingPointValid() const
    {
      return m_bindingPoint != s_allocator.Invalid;
    }

    inline constexpr GLuint getBindingPoint() const
    {
      return m_bindingPoint;
    }

  protected:

    inline constexpr GLuint generateAndAssignBindingPoint()
    {
      if (!isBindingPointValid())
      {
        m_bindingPoint = s_allocator.next();
      }

      return m_bindingPoint;
    }

  private:
    inline static BindingPointAllocator<GLuint> s_allocator = { 0, 64 };

    GLuint m_bindingPoint = s_allocator.Invalid;
  };

  class StorageBuffer : public StorageBufferBase, public Buffer<GL_SHADER_STORAGE_BUFFER, BufferBehaviour::Static>
  {
  public:
    using Parent = Buffer<GL_SHADER_STORAGE_BUFFER, BufferBehaviour::Static>;
  public:

    inline constexpr StorageBuffer()
    {

    }

    inline virtual ~StorageBuffer()
    {
      
    }
    
    StorageBuffer(const StorageBuffer&) = delete;
    StorageBuffer& operator=(const StorageBuffer&) = delete;

    inline constexpr StorageBuffer(StorageBuffer&& other) noexcept :
      StorageBufferBase(std::move(other)), Parent(std::move(other))
    {
    }

    inline StorageBuffer& operator=(StorageBuffer&& other) noexcept
    {
      StorageBufferBase::operator=(std::move(other));
      Parent::operator=(std::move(other));

      return *this;
    }

    inline void reset(GLsizei capacity, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Parent::reset(capacity, usage);
      // If no binding point has been given, we generate one and bind this ssbo to it
      // If one was already given, we let the ssbo on its binding point
      if (!isBindingPointValid())
      {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, generateAndAssignBindingPoint(), this->getRenderID());
      }
    }

  };

  class AutoResizeStorageBuffer : public StorageBufferBase, public Buffer<GL_SHADER_STORAGE_BUFFER, BufferBehaviour::AutoResize>
  {
  public:
    using Parent = Buffer<GL_SHADER_STORAGE_BUFFER, BufferBehaviour::AutoResize>;
  public:

    inline constexpr AutoResizeStorageBuffer()
    {

    }

    inline virtual ~AutoResizeStorageBuffer()
    {
      
    }

    AutoResizeStorageBuffer(const AutoResizeStorageBuffer&) = delete;
    AutoResizeStorageBuffer& operator=(const AutoResizeStorageBuffer&) = delete;

    inline constexpr AutoResizeStorageBuffer(AutoResizeStorageBuffer&& other) noexcept :
      StorageBufferBase(std::move(other)), Parent(std::move(other))
    {
    }

    inline AutoResizeStorageBuffer& operator=(AutoResizeStorageBuffer&& other) noexcept
    {
      StorageBufferBase::operator=(std::move(other));
      Parent::operator=(std::move(other));

      return *this;
    }

    inline void reset(GLsizei capacity, GLenum usage = GL_DYNAMIC_DRAW)
    {
      Parent::reset(capacity, usage);
      // If no binding point has been given, we generate one and bind this ssbo to it
      // If one was already given, we let the ssbo on its binding point
      if (!isBindingPointValid())
      {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, generateAndAssignBindingPoint(), this->getRenderID());
      }
    }

  };

} // namespace vrm::gl
