#include "Vroom/Render/Abstraction/Buffer.h"

using namespace vrm::gl;

Buffer::Buffer()
{

}

Buffer::~Buffer() noexcept
{
  release();
}

Buffer& Buffer::operator=(Buffer&& other)
{
  if (this != &other)
  {
    release();

    m_renderId = other.m_renderId;
    m_desc = other.m_desc;

    other.m_renderId = 0;
    other.m_desc = {};
  }

  return *this;
}

Buffer::Buffer(Buffer&& other)
{
  *this = std::move(other);
}

void Buffer::_create(const Desc& description, const void* data)
{
  release();

  glGenBuffers(1, &m_renderId);
  m_desc = description;

  GLbitfield flags = 0
    | (m_desc.allowMapReading ? GL_MAP_READ_BIT        : 0)
    | (m_desc.allowMapWriting ? GL_MAP_WRITE_BIT       : 0)
    | (m_desc.allowDataUpload ? GL_DYNAMIC_STORAGE_BIT : 0)
  ;

  Bind(*this, GL_ARRAY_BUFFER);
  glBufferStorage(GL_ARRAY_BUFFER, m_desc.capacity, data, flags);
  Unbind(GL_ARRAY_BUFFER);
}

void Buffer::release() noexcept
{
  if (m_renderId != 0)
  {
    glDeleteBuffers(1, &m_renderId);
    m_renderId = 0;
    m_desc = {};
  }
}

void Buffer::Bind(GLuint renderId, GLenum target)
{
  glBindBuffer(target, renderId);
}
