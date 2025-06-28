#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Vroom/Core/Assert.h"

namespace vrm
{

  template <typename T, typename NameType = std::string>
  class ResourcePool
  {
  public:

    inline ResourcePool()
    {

    }

    inline ~ResourcePool()
    {

    }

    inline void clear()
    {
      m_data.clear();
    }

    template <typename... Args>
    inline T* emplace(const NameType& name, Args&&... args)
    {
      return emplace<T>(name, std::forward<Args>(args)...);
    }

    template <typename Child, typename... Args>
      requires (std::is_base_of_v<T, Child> || std::is_same_v<Child, T>)
    inline Child* emplace(const NameType& name, Args&&... args)
    {
      VRM_ASSERT(!m_data.contains(name));
      auto ptr = std::make_unique<Child>(std::forward<Args>(args)...);
      Child* rawPtr = ptr.get();

      m_data.emplace(name, std::move(ptr));

      return rawPtr;
    }

    inline T* get(const NameType& name)
    {
      auto it = m_data.find(name);
      VRM_ASSERT(it != m_data.end());

      return it->second.get();
    }

    inline const T* get(const NameType& name) const
    {
      auto it = m_data.find(name);
      VRM_ASSERT(it != m_data.end());

      return it->second.get();
    }

    inline void release(const NameType& name)
    {
      auto it = m_data.find(name);
      VRM_ASSERT(it != m_data.end());

      return m_data.erase(it);
    }

    inline bool contains(const NameType& name) const
    {
      return m_data.contains(name);
    }

  public:
    std::unordered_map<NameType, std::unique_ptr<T>> m_data;
  };

} // namespace vrm
