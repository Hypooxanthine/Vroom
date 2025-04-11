#pragma once

#include <vector>
#include <set>

#include "Vroom/Render/Abstraction/StorageBuffer.h"
#include "Vroom/Core/Assert.h"

namespace vrm
{

  template <typename GpuType, typename IdType, GLuint MaxElements = 0>
  class StorageBufferRegistry
  {
  public:

    inline StorageBufferRegistry(gl::AutoResizeStorageBuffer* storageBuffer, GLuint offset)
      : m_ssbo(storageBuffer), m_offset(offset)
    {
    }

    inline void submit(const IdType& id, const GpuType& data)
    {
      if constexpr (MaxElements > 0)
      {
        VRM_ASSERT_MSG(m_data.size() < MaxElements, "Maximum element count is reached");
      }

      VRM_ASSERT_MSG(!m_idToAddress.contains(id), "id already registered");

      size_t addr = getValidAddress();
      m_idToAddress[id] = addr;
      m_activeElements.at(addr) = true;

      update(id, data);

      markDirty();
    }

    inline void update(const IdType& id, const GpuType& data)
    {
      auto &element = getElement(id);
      element = data;
    }

    inline void remove(const IdType& id)
    {
      VRM_ASSERT_MSG(m_idToAddress.contains(id), "id not registered");

      size_t freed = m_idToAddress.at(id);

      m_idToAddress.erase(id);
      m_freeAddresses.emplace(freed);
      m_activeElements.at(freed) = false;

      markDirty();
    }

    inline void updateStorageBuffer()
    {
      if (isDirty())
      {
        // Data compaction
        std::vector<GpuType> newData;
        newData.reserve(m_data.size() - m_freeAddresses.size());
        std::unordered_map<size_t, size_t> remap;

        for (size_t i = 0; i < m_activeElements.size(); ++i)
        {
          if (m_activeElements.at(i))
          {
            remap[i] = newData.size();
            newData.emplace_back(m_data.at(i));
          }
        }

        for (auto& [id, addr] : m_idToAddress)
        {
          addr = remap.at(addr);
        }

        m_data = std::move(newData);
        m_activeElements.assign(m_data.size(), true); // All lights are active now
        m_freeAddresses.clear();

        m_dirty = false;
      }
      
      updateGpuData();
    }

  protected:

    inline size_t getValidAddress()
    {
      size_t out;
      if (m_freeAddresses.empty())
      {
        out = m_data.size();
        m_data.emplace_back();
        m_activeElements.emplace_back();
      }
      else
      {
        auto it = m_freeAddresses.begin();
        out = *it;
        m_freeAddresses.erase(it);
      }

      return out;
    }

    inline GpuType& getElement(const IdType& id)
    {
      VRM_ASSERT_MSG(m_idToAddress.contains(id), "id not registered");

      return m_data.at(m_idToAddress.at(id));
    }

    inline void markDirty()
    {
      m_dirty = true;
    }

    inline bool isDirty() const { return m_dirty; }

    inline void updateGpuData()
    {
      uint32_t elementsCount = static_cast<uint32_t>(m_data.size());
      m_ssbo->setData(&elementsCount, sizeof(elementsCount), m_offset);

      m_ssbo->setData(m_data.data(), static_cast<GLsizei>(m_data.size() * sizeof(GpuType)), m_offset + sizeof(elementsCount));
    }

  private:
    gl::AutoResizeStorageBuffer* m_ssbo;
    GLuint m_offset;

    std::vector<GpuType> m_data;
    std::unordered_map<IdType, size_t> m_idToAddress;
    std::set<size_t> m_freeAddresses;
    std::vector<bool> m_activeElements;

    bool m_dirty = false;
  };

} // namespace vrm
