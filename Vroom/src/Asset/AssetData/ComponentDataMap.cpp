#include "Vroom/Asset/AssetData/ComponentDataMap.h"

using namespace vrm;

ComponentDataMap& ComponentDataMap::operator=(const ComponentDataMap& other)
{
  if (this != &other)
  {
    m_data.clear();
    
    for (const auto& [key, comp] : other.m_data)
    {
      m_data[key].reset(comp->clone());
    }
  }

  return *this;
}

ComponentDataMap::ComponentDataMap(const ComponentDataMap& other)
{
  *this = other;
}

ComponentDataMap& ComponentDataMap::operator=(ComponentDataMap&& other)
{
  if (this != &other)
  {
    m_data = std::move(other.m_data);
  }

  return *this;
}

ComponentDataMap::ComponentDataMap(ComponentDataMap&& other)
{
  *this = std::move(other);
}
