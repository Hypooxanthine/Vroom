#pragma once

#include <vector>
#include <set>

#include "Vroom/DataStructure/LinearRegistry.h"
#include "Vroom/Render/AutoBuffer.h"
#include "Vroom/Core/Assert.h"

namespace vrm
{

  /**
   * @brief Keeps an ssbo updated with a LinearRegistry. SSBO layout will be following:
   *  buffer SSBO_Block
   *  {
   *    uint elementCount;
   *    GpuType data[MaxElements]; // if MaxElements > 0
   *    GpuType data[];            // if MaxElements == 0
   *  } SSBO_Name;
   * 
   * @tparam GpuType The data type to be sent on gpu
   * @tparam IdType The identifier type, to retrieve and update elements (std::string, size_t, ...)
   * @tparam MaxElements If 0, no check will be made on the number of registered elements
   */
  template <typename GpuType, typename IdType, GLuint MaxElements = 0, size_t Alignment = GpuType::Alignment>
  class StorageBufferRegistry
  {
  public:

    using RegistryType = LinearRegistry<GpuType, IdType>;

  public:

    inline StorageBufferRegistry(render::AutoBuffer* storageBuffer, GLuint offset)
      : m_ssbo(storageBuffer), m_offset(offset)
    {
    }

    inline const RegistryType& getCPURegistry() const { return m_data; }

    inline void startRegistering()
    {
      m_data.startRegistering();
    }

    inline void submit(const IdType& id, const GpuType& data)
    {
      if constexpr (MaxElements > 0)
      {
        VRM_ASSERT_MSG(m_data.getElementCount() < MaxElements, "Maximum element count is reached");
      }
      
      m_data.submit(id, data);
    }

    inline void notifyUsed(const IdType& id)
    {
      m_data.notifyUsed(id);
    }

    inline void endRegistering()
    {
      m_data.endRegistering();

      if (m_data.wasJustModified())
      {
        updateGpuData();
      }
    }

  protected:

    inline void updateGpuData()
    {
      uint32_t elementsCount = static_cast<uint32_t>(m_data.getElementCount());
      size_t counterSize = std::max(sizeof(elementsCount), Alignment);

      m_ssbo->ensureCapacity(m_offset + sizeof(elementsCount) + m_data.getElementCount() * sizeof(GpuType), true);
      std::span<uint8_t> map = m_ssbo->mapWriteOnly(m_offset, sizeof(elementsCount) + m_data.getElementCount() * sizeof(GpuType));

      std::memcpy(map.data(),               &elementsCount,      sizeof(elementsCount));
      std::memcpy(map.data() + counterSize, m_data.getRawData(), m_data.getElementCount() * sizeof(GpuType));

      // m_ssbo->setData(&elementsCount, sizeof(elementsCount), m_offset);
      
      // m_ssbo->setData(m_data.getRawData(), static_cast<GLsizei>(m_data.getElementCount() * sizeof(GpuType)), m_offset + counterSize);

      m_ssbo->unmap();
    }

  private:
    RegistryType m_data;

    render::AutoBuffer* m_ssbo;
    GLuint m_offset;
  };

} // namespace vrm
