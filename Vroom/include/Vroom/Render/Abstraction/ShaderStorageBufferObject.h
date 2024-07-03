#pragma once

class ShaderStorageBufferObject
{
public:
    ShaderStorageBufferObject();
    ~ShaderStorageBufferObject();

    void bind();
    void unbind();

    void setData(const void* data, size_t size);

    void setSubData(const void* data, size_t size, size_t offset);

    void setBindingPoint(unsigned int bindingPoint);

    unsigned int getBindingPoint() const;

private:
    unsigned int m_RendererID;
    unsigned int m_BindingPoint;
};