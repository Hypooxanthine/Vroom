#pragma once

#include "Vroom/Core/Assert.h"

#include "Vroom/Render/Abstraction/ShaderStorageBufferObject.h"

namespace vrm::gl
{

  class DynamicSSBO
  {
  public:
    DynamicSSBO() = default;
    DynamicSSBO(const DynamicSSBO &) = default;
    DynamicSSBO(DynamicSSBO &&) = default;
    ~DynamicSSBO() = default;

    DynamicSSBO &operator=(const DynamicSSBO &) = default;
    DynamicSSBO &operator=(DynamicSSBO &&) = default;

    void bind() const;
    void unbind() const;

    void setBindingPoint(int bindingPoint);

    int getCapacity() const;

    void reserve(int capacity);

    void clear();

    void setSubData(const void *data, int size, int offset);
    void setData(const void *data, int size);

    /**
     * @brief Set data from a RawShaderData object.
     *
     * @tparam RawShaderData The type of the RawShaderData object. Must have a getData() method that returns a vector of pairs of const void* and size_t.
     * @param data The RawShaderData object.
     */
    template <typename RawShaderData>
    void setData(const RawShaderData &data)
    {
      auto ptrAndSize = data.getData();
      size_t totalSize = 0;

      for (const auto &[_, dataSize] : ptrAndSize)
      {
        totalSize += dataSize;
      }

      reserve(static_cast<int>(totalSize));

      size_t offset = 0;
      for (const auto &[dataPtr, dataSize] : ptrAndSize)
      {
        setSubData(dataPtr, static_cast<int>(dataSize), static_cast<int>(offset));
        offset += dataSize;
      }
    }

  private:
    gl::ShaderStorageBufferObject m_SSBO;
    int m_SSBOCapacity = 0;
  };

} // namespace vrm
