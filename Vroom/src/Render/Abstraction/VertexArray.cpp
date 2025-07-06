#include "Vroom/Render/Abstraction/VertexArray.h"

#include "Vroom/Render/Abstraction/Buffer.h"

using namespace vrm::gl;


VertexArray::VertexArray()
{

}

VertexArray::~VertexArray() noexcept
{
  release();
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
  if (this != &other)
  {
    release();

    m_renderId = other.m_renderId;
    m_attribs = std::move(other.m_attribs);

    other.m_renderId = 0;
    other.m_attribs = {};
  }

  return *this;
}

VertexArray::VertexArray(VertexArray&& other)
{
  *this = std::move(other);
}

void VertexArray::create()
{
  release();

  glGenVertexArrays(1, &m_renderId);
}

void VertexArray::release() noexcept
{
  if (m_renderId != 0)
  {
    glDeleteVertexArrays(1, &m_renderId);
    m_renderId = 0;
  }
}

void VertexArray::enableAttribute(uint32_t attribIndex, const Attribute& attrib, const Buffer& vertexBuffer)
{
  m_attribs.at(attribIndex).attrib = attrib;
  m_attribs.at(attribIndex).enabled = true;
  
  VertexArray::Bind(*this);
  glEnableVertexArrayAttrib(m_renderId, attribIndex);

  Buffer::Bind(vertexBuffer, GL_ARRAY_BUFFER);
  glVertexAttribPointer(
    attribIndex,
    attrib.componentCount,
    attrib.componentType,
    attrib.normalized ? GL_TRUE : GL_FALSE,
    attrib.vertexBufferStrideBytes,
    reinterpret_cast<const void*>(static_cast<std::uintptr_t>(attrib.vertexBufferOffsetBytes))
  );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  Buffer::Unbind(GL_ARRAY_BUFFER);
  VertexArray::Unbind();
}

void VertexArray::disableAttribute(uint32_t attribIndex)
{
  glDisableVertexArrayAttrib(m_renderId, attribIndex);
}

void VertexArray::Bind(GLuint renderId)
{
  glBindVertexArray(renderId);
}

