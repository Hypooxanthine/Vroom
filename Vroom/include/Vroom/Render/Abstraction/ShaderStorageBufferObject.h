#pragma once

#include <GL/glew.h>

class ShaderStorageBufferObject
{
public:
    enum class AccessType
    {
        READ_ONLY,
        WRITE_ONLY,
        READ_WRITE
    };
public:
    ShaderStorageBufferObject();
    ShaderStorageBufferObject(const ShaderStorageBufferObject&) = delete;
    ShaderStorageBufferObject(ShaderStorageBufferObject&&);
    ~ShaderStorageBufferObject();

    ShaderStorageBufferObject& operator=(const ShaderStorageBufferObject&) = delete;
    ShaderStorageBufferObject& operator=(ShaderStorageBufferObject&&);

    void bind() const;
    void unbind() const;

    void setData(const void* data, int size);
    void setSubData(const void* data, int size, int offset);

    void clear();

    void setBindingPoint(unsigned int bindingPoint);

    void* mapBuffer(AccessType accessType);
    void unmapBuffer();

    unsigned int getBindingPoint() const;

    bool hasBindingPoint() const { return m_HasBindingPoint; }

private:
    constexpr static GLenum AccessTypeToGL(AccessType accessType);

private:
    unsigned int m_RendererID;
    bool m_HasBindingPoint = false;
    unsigned int m_BindingPoint;
};