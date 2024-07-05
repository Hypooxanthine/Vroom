#include "Vroom/Render/Abstraction/DynamicSSBO.h"

namespace vrm
{

void DynamicSSBO::bind() const
{
    m_SSBO.bind();
}

void DynamicSSBO::unbind() const
{
    m_SSBO.unbind();
}

void DynamicSSBO::setBindingPoint(int bindingPoint)
{
    m_SSBO.setBindingPoint(bindingPoint);
}

int DynamicSSBO::getSize() const { return m_SSBOSize; }
int DynamicSSBO::getCapacity() const { return m_SSBOCapacity; }

void DynamicSSBO::reserve(int capacity)
{
    if (capacity < m_SSBOCapacity)
        return;

    // Creating a new SSBO with the requested capacity
    ShaderStorageBufferObject newSSSBO;
    newSSSBO.setData(nullptr, capacity);

    // Copying the data from the old SSBO to the new one
    void* data = m_SSBO.mapBuffer(ShaderStorageBufferObject::AccessType::READ_ONLY);
    newSSSBO.setSubData(data, m_SSBOSize, 0);
    m_SSBO.unmapBuffer();

    // Replacing the old SSBO with the new one
    m_SSBO = std::move(newSSSBO);

    m_SSBOCapacity = capacity;
}

void DynamicSSBO::shrink()
{
    if (m_SSBOSize == m_SSBOCapacity)
        return;

    ShaderStorageBufferObject newSSBO;

    // Here, we just have to set the data of the new SSBO with the data of the old SSBO
    // It will allocate the exact size of the data for us
    void* data = m_SSBO.mapBuffer(ShaderStorageBufferObject::AccessType::READ_ONLY);
    newSSBO.setData(data, m_SSBOSize);
    m_SSBO.unmapBuffer();

    m_SSBO = std::move(newSSBO);

    m_SSBOCapacity = m_SSBOSize;
}

void DynamicSSBO::clear()
{
    m_SSBO.clear();
    m_SSBOSize = 0;
    m_SSBOCapacity = 0;
}

void DynamicSSBO::setSubData(const void* data, int size, int offset)
{
    // If the new data doesn't fit in the current SSBO, we need to reallocate
    // We reserve twice the size of the new data to avoid reallocating too often
    if (size + offset > m_SSBOCapacity)
        reserve((size + offset) * 2);

    m_SSBO.setSubData(data, size, offset);

    if (size + offset > m_SSBOSize)
        m_SSBOSize = size + offset;
}

void DynamicSSBO::setData(const void* data, int size, bool shrink)
{
    if (size > m_SSBOCapacity || (shrink && size < m_SSBOCapacity))
    {
        // If the new size is bigger than the current capacity, we need to reallocate
        // If we want to shrink the SSBO, we also need to reallocate (only if the new size is strictly smaller than the current capacity,
        // because we don't want to reallocate if the new size is equal to the current capacity, even if we want to shrink the SSBO)
        m_SSBO.setData(data, size);
        m_SSBOCapacity = size;
    }
    else
    {
        // If the new size is smaller than the current capacity, we can just set the data
        m_SSBO.setSubData(data, size, 0);
        // Capacity remains the same
    }

    m_SSBOSize = size;
}

} // namespace vrm