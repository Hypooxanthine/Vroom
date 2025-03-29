#include "Vroom/Render/Abstraction/IndexBuffer.h"

#include "Vroom/Render/Abstraction/GLCall.h"

using namespace vrm;
using namespace vrm::gl;

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_RendererID(0), m_Count(count)
{
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer &&other)
    : m_RendererID(other.m_RendererID), m_Count(other.m_Count)
{
  other.m_RendererID = 0;
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other)
{
  if (this != &other)
  {
    m_RendererID = other.m_RendererID;
    m_Count = other.m_Count;
    other.m_RendererID = 0;
  }

  return *this;
}

IndexBuffer::~IndexBuffer()
{
  GLCall_nothrow(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
