#pragma once

#include <span>

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/GLCall.h"

namespace vrm::gl
{
  
  class Buffer
  {
  public:

    struct Desc
    {
      GLsizeiptr capacity = 0;
      bool allowMapReading = false;
      bool allowMapWriting = false;
      bool allowDataUpload = false;
    };

  public:

    Buffer();
    ~Buffer() noexcept;

    Buffer& operator=(const Buffer&) = delete;
    Buffer(const Buffer&) = delete;

    Buffer& operator=(Buffer&& other);
    Buffer(Buffer&& other);

    template <typename ArrayElement, size_t ElementCount>
    void create(const Desc& description, std::span<ArrayElement const, ElementCount> initialData)
    {
      VRM_ASSERT(initialData.data() == nullptr || initialData.size_bytes() <= description.capacity);

      _create(description, initialData.data());
    }

    void create(const Desc& description)
    {
      _create(description, nullptr);
    }

    void release() noexcept;

    static void Bind(GLuint renderId, GLenum target);
    inline static void Bind(const Buffer& buffer, GLenum target) { Bind(buffer.getRenderId(), target); }
    inline static void Unbind(GLenum target) { Bind(0, target); }

    inline GLuint getRenderId() const { return m_renderId; }
    inline Desc getDescription() const { return m_desc; }
    inline GLsizeiptr getCapacity() const { return m_desc.capacity; }

  private:

    void _create(const Desc& description, const void* data);

  private:

    GLuint m_renderId = 0;
    Desc m_desc = {};

  };

} // namespace vrm::gl
