#pragma once

#include <span>

#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::render
{

  class AutoBuffer
  {
  public:

    AutoBuffer();
    ~AutoBuffer();

    AutoBuffer& operator=(const AutoBuffer&) = delete;
    AutoBuffer(const AutoBuffer&) = delete;

    AutoBuffer& operator=(AutoBuffer&& other);
    AutoBuffer(AutoBuffer&& other);
    
    void ensureCapacity(size_t capacity, bool keepData = false);

    template <typename T = uint8_t>
    std::span<T> mapWriteOnly(uint32_t offset, uint32_t sizeBytes, bool discardData = true) const
    {
      T* typedPtr = reinterpret_cast<T*>(_mapWriteOnly(offset, sizeBytes, discardData));
      return std::span<T>(typedPtr, sizeBytes / sizeof(T));
    }
    
    std::span<uint8_t> mapWriteOnly(bool discardData = true) const
    {
      return mapWriteOnly(0, m_buffer.getCapacity(), discardData);
    }

    void unmap() const;

    const gl::Buffer& getBuffer() const { return m_buffer; }

  private:

    void* _mapWriteOnly(uint32_t offset, uint32_t size, bool discardData) const;

  private:
    gl::Buffer m_buffer;
    
  };

}