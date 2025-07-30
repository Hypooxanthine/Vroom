#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Vroom/Asset/AssetData/ComponentData.h"

namespace vrm
{

  class ComponentDataMap
  {
  public:

    using container = std::unordered_map<std::type_index, std::unique_ptr<ComponentData>>;
    using key_type = container::key_type;
    using value_type = container::value_type;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

  public:

    ComponentDataMap() = default;
    ~ComponentDataMap() = default;

    ComponentDataMap& operator=(const ComponentDataMap& other);
    ComponentDataMap(const ComponentDataMap& other);

    ComponentDataMap& operator=(ComponentDataMap&& other);
    ComponentDataMap(ComponentDataMap&& other);

    inline bool empty() const { return m_data.empty(); }

    inline std::unique_ptr<ComponentData>& operator[](const key_type& key) { return m_data[key]; }

    inline iterator begin() { return m_data.begin(); }
    inline iterator end() { return m_data.end(); }

    inline const_iterator begin() const { return m_data.begin(); }
    inline const_iterator end() const { return m_data.end(); }

    inline const_iterator cbegin() const { return m_data.cbegin(); }
    inline const_iterator cend() const { return m_data.cend(); }

    inline iterator find(const key_type& key) { return m_data.find(key); }
    inline const_iterator find(const key_type& key) const { return m_data.find(key); }

  private:
  
    container m_data;

  };

} // namespace vrm
