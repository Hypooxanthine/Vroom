#include "Vroom/Render/Abstraction/ShaderStorageBufferObject.h"

#include "Vroom/Render/Abstraction/GLCall.h"

ShaderStorageBufferObject::ShaderStorageBufferObject()
{
    GLCall(glGenBuffers(1, &m_RendererID));
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
    GLCall_nothrow(glDeleteBuffers(1, &m_RendererID));
}

void ShaderStorageBufferObject::bind()
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
}

void ShaderStorageBufferObject::unbind()
{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ShaderStorageBufferObject::setData(const void* data, size_t size)
{
    bind();
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void ShaderStorageBufferObject::setSubData(const void* data, size_t size, size_t offset)
{
    bind();
    GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));
}

void ShaderStorageBufferObject::setBindingPoint(unsigned int bindingPoint)
{
    m_BindingPoint = bindingPoint;
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererID));
}

unsigned int ShaderStorageBufferObject::getBindingPoint() const
{
    return m_BindingPoint;
}

