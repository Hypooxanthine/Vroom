#include "Vroom/Render/AutoBuffer.h"

using namespace vrm::render;

AutoBuffer::AutoBuffer()
{

}

AutoBuffer::~AutoBuffer()
{

}

AutoBuffer& AutoBuffer::operator=(AutoBuffer&& other)
{
  if (this != &other)
  {
    m_buffer = std::move(other.m_buffer);
  }

  return *this;
}

AutoBuffer::AutoBuffer(AutoBuffer&& other)
{
  *this = std::move(other);
}

void AutoBuffer::ensureCapacity(size_t capacity, bool keepData)
{
  if (m_buffer.getCapacity() >= capacity)
  {
    return;
  }

  gl::Buffer::Desc desc;
  desc.capacity = capacity;
  desc.allowMapWriting = true;
  
  if (keepData && m_buffer.getCapacity() > 0)
  {
    gl::Buffer intermediate;
    intermediate.create(desc);

    glCopyNamedBufferSubData(
      m_buffer.getRenderId(), intermediate.getRenderId(), // from -> to
      0, 0, m_buffer.getCapacity()
    );

    m_buffer = std::move(intermediate);
  }
  else
  {
    m_buffer.create(desc);
  }
}

void* AutoBuffer::_mapWriteOnly(uint32_t offset, uint32_t size, bool discardData) const
{
  VRM_ASSERT(offset + size <= m_buffer.getCapacity());

  GLbitfield flags = 0
    | GL_MAP_WRITE_BIT
    | (discardData ? GL_MAP_INVALIDATE_RANGE_BIT : 0)
  ;

  GLCall(void* ptr = glMapNamedBufferRange(m_buffer.getRenderId(), static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), flags));
  return ptr;
}

void AutoBuffer::unmap() const
{
  glUnmapNamedBuffer(m_buffer.getRenderId());
}
