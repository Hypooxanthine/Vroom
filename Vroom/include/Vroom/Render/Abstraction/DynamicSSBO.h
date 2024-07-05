#pragma once

#include "Vroom/Core/Assert.h"

#include "Vroom/Render/Abstraction/ShaderStorageBufferObject.h"

namespace vrm
{

class DynamicSSBO
{
public:
    DynamicSSBO() = default;
    DynamicSSBO(const DynamicSSBO&) = default;
    DynamicSSBO(DynamicSSBO&&) = default;
    ~DynamicSSBO() = default;

    DynamicSSBO& operator=(const DynamicSSBO&) = default;
    DynamicSSBO& operator=(DynamicSSBO&&) = default;

    void bind() const;
    void unbind() const;

    void setBindingPoint(int bindingPoint);

    int getSize() const;
    int getCapacity() const;

    void reserve(int capacity);

    void shrink();
    void clear();

    void setSubData(const void* data, int size, int offset);
    void setData(const void* data, int size, bool shrink = false);

private:
    ShaderStorageBufferObject m_SSBO;
    int m_SSBOSize = 0, m_SSBOCapacity = 0;
};

} // namespace vrm
