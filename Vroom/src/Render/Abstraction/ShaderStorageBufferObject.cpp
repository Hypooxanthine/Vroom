#include "Vroom/Render/Abstraction/ShaderStorageBufferObject.h"

#include "Vroom/Render/Abstraction/GLCall.h"

using namespace vrm;
using namespace vrm::gl;

ShaderStorageBufferObject::ShaderStorageBufferObject()
{
  GLCall(glGenBuffers(1, &m_RendererID));
}

ShaderStorageBufferObject::ShaderStorageBufferObject(ShaderStorageBufferObject &&other)
    : m_RendererID(other.m_RendererID), m_BindingPoint(other.m_BindingPoint)
{
  other.m_RendererID = 0;
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
  GLCall_nothrow(glDeleteBuffers(1, &m_RendererID));
}

ShaderStorageBufferObject &ShaderStorageBufferObject::operator=(ShaderStorageBufferObject &&other)
{
  if (this != &other)
  {
    this->~ShaderStorageBufferObject();
    m_RendererID = other.m_RendererID;
    m_BindingPoint = other.m_BindingPoint;
    other.m_RendererID = 0;
  }

  return *this;
}

void ShaderStorageBufferObject::bind() const
{
  GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
}

void ShaderStorageBufferObject::unbind() const
{
  GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ShaderStorageBufferObject::setData(const void *data, int size)
{
  bind();
  GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void ShaderStorageBufferObject::setSubData(const void *data, int size, int offset)
{
  bind();
  GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));
}

void ShaderStorageBufferObject::clear()
{
  setData(nullptr, 0);
}

void ShaderStorageBufferObject::setBindingPoint(unsigned int bindingPoint)
{
  m_BindingPoint = bindingPoint;
  GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererID));
  m_HasBindingPoint = true;
}

unsigned int ShaderStorageBufferObject::getBindingPoint() const
{
  return m_BindingPoint;
}

void *ShaderStorageBufferObject::mapBuffer(AccessType accessType)
{
  bind();
  GLCall(void *ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, AccessTypeToGL(accessType)));
  return ptr;
}

void ShaderStorageBufferObject::unmapBuffer()
{
  bind();
  GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}

constexpr GLenum ShaderStorageBufferObject::AccessTypeToGL(AccessType accessType)
{
  switch (accessType)
  {
  case AccessType::READ_ONLY:
    return GL_READ_ONLY;
  case AccessType::WRITE_ONLY:
    return GL_WRITE_ONLY;
  case AccessType::READ_WRITE:
    return GL_READ_WRITE;
  default:
    return GL_READ_ONLY;
  }
}
