#pragma once

#include <cstddef>
#include <span>
#include <vector>

#include "Vroom/Render/SSBO430Layout.h"

namespace vrm::render
{

/**
 * @brief This class mimics a C++ struct with a specific layout rules set. RAII
 * not supported.
 *
 */
class StructuredBuffer
{
public:

  StructuredBuffer()  = default;
  ~StructuredBuffer() = default;

  StructuredBuffer& operator=(const StructuredBuffer& other) = default;
  StructuredBuffer(const StructuredBuffer& other)            = default;

  StructuredBuffer& operator=(StructuredBuffer&& other) = default;
  StructuredBuffer(StructuredBuffer&& other)            = default;

  inline void setup(const SSBO430Layout& layout, size_t elemCount)
  {
    m_layout    = layout;
    m_arraySize = elemCount;
    m_data.assign(elemCount * m_layout.getAlignedSize(), std::byte(0));
  }

  template <typename T>
  inline void setAttribute(std::span<T const>           data,
                           const SSBO430Layout::Attrib& attribute,
                           size_t                       arrayIndex);

  template <typename T>
  inline void setAttribute(const T&                     data,
                           const SSBO430Layout::Attrib& attribute,
                           size_t                       arrayIndex);

  inline const SSBO430Layout& getLayout() const { return m_layout; }
  inline size_t               getArraySize() const { return m_arraySize; }

private:

  void _setAttributeData(std::span<std::byte const>   data,
                         const SSBO430Layout::Attrib& attribute,
                         size_t                       arrayIndex);

private:

  SSBO430Layout          m_layout;
  std::vector<std::byte> m_data;
  size_t                 m_arraySize = 0;
};

template <typename T>
inline void StructuredBuffer::setAttribute(
  const T& data, const SSBO430Layout::Attrib& attribute, size_t arrayIndex)
{
  std::byte const* dataPtr = reinterpret_cast<std::byte const*>(&data);
  _setAttributeData(std::span<std::byte const>(dataPtr, sizeof(T)), attribute,
                    arrayIndex);
}

template <typename T>
inline void
StructuredBuffer::setAttribute(std::span<T const>           data,
                               const SSBO430Layout::Attrib& attribute,
                               size_t                       arrayIndex)
{
  std::byte const* dataPtr = reinterpret_cast<std::byte const*>(data.data());
  _setAttributeData(std::span<std::byte const>(dataPtr, data.size_bytes()),
                    attribute, arrayIndex);
}

} // namespace vrm::render
