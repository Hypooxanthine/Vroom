#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "Vroom/Core/Assert.h"

namespace vrm
{

  template <typename T, typename IdType>
  class LinearRegistry
  {
  public:
    friend class test_LinearRegistry;
    using ContainerType = std::vector<T>;
    using KeySet = std::unordered_set<IdType>;

  public:

    inline LinearRegistry()
    {

    }

    inline ~LinearRegistry()
    {

    }

    inline void reset()
    {
      m_data.clear();
      m_indexMap.clear();
      m_idMap.clear();
      m_waitingKeys.clear();
      m_confirmedKeys.clear();
      m_isRegistering = false;
      m_modified = false;
    }

    inline bool contains(const IdType& id) const
    {
      return m_indexMap.contains(id);
    }

    inline bool isRegistering() const { return m_isRegistering; }

    inline void startRegistering()
    {
      VRM_ASSERT_MSG(m_isRegistering == false, "Already registering");
      m_isRegistering = true;

      std::swap(m_waitingKeys, m_confirmedKeys);
      m_confirmedKeys.clear();
      m_modified = false;
    }

    template <typename uT>
      requires std::is_same_v<std::decay_t<uT>, T>
    inline void submit(const IdType& id, uT&& element)
    {
      notifyUsed(id);

      if (m_indexMap.contains(id))
      {
        m_data.at(m_indexMap.at(id)) = std::forward<uT>(element);
      }
      else
      {
        m_indexMap[id] = m_data.size();
        m_idMap[m_data.size()] = id;
        m_data.emplace_back(std::forward<uT>(element));
      }

      m_modified = true;
    }

    inline void notifyUsed(const IdType& id)
    {
      VRM_ASSERT_MSG(isRegistering(), "Wasn't registering");
      m_waitingKeys.erase(id);
      m_confirmedKeys.insert(id);
    }

    inline void endRegistering()
    {
      VRM_ASSERT_MSG(m_isRegistering == true, "Wasn't registering");
      m_isRegistering = false;

      if (m_waitingKeys.empty())
      {
        return;
      }

      m_modified = true;
      std::set<size_t> sortedIdsToDelete;

      for (const auto& missingId : m_waitingKeys)
      {
        sortedIdsToDelete.emplace(m_indexMap.at(missingId));
        m_indexMap.erase(missingId);
      }

      for (auto it = sortedIdsToDelete.rbegin(); it != sortedIdsToDelete.rend(); ++it)
      {
        const size_t location = *it;
        const size_t end = m_data.size() - 1;

        if (location < end)
        {
          std::swap(m_data.at(location), m_data.at(end));
          m_idMap.at(location) = std::move(m_idMap.at(end));
          m_indexMap.at(m_idMap.at(location)) = location;
        }

        m_data.pop_back();
        m_idMap.erase(end);
      }
    }

    inline size_t getElementCount() const { return m_data.size(); }

    inline const T* getRawData() const { return m_data.data(); }

    inline const T& at(size_t index) const { return m_data.at(index); }

    inline const bool wasJustModified() const
    {
      VRM_ASSERT_MSG(m_isRegistering == false, "Registry data is undefined while registering. Call endregistering() first");

      return m_modified;
    }

    inline const KeySet& getRegisteredKeys() const
    {
      VRM_ASSERT_MSG(m_isRegistering == false, "Registry data is undefined while registering. Call endregistering() first");

      return m_confirmedKeys;
    }

    inline const KeySet& getJustRemovedKeys() const
    {
      VRM_ASSERT_MSG(m_isRegistering == false, "Registry data is undefined while registering. Call endregistering() first");

      return m_waitingKeys;
    }

  public:

    class const_iterator
    {
    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair<const IdType&, const T&>;
      using difference_type = std::ptrdiff_t;
      using pointer = void;
      using reference = value_type;

      const_iterator(size_t index, const LinearRegistry* registry)
        : m_index(index), m_registry(registry)
      {
      }

      const_iterator& operator=(const const_iterator& other)
      {
        if (this != &other)
        {
          m_registry = other.m_registry;
          m_index = other.m_index;
        }

        return *this;
      }

      const_iterator& operator++()
      {
        ++m_index;
        return *this;
      }

      const_iterator& operator+=(size_t count)
      {
        m_index += count;
        return *this;
      }

      bool operator==(const const_iterator& other) const
      {
        return m_index == other.m_index;
      }

      bool operator!=(const const_iterator& other) const
      {
        return m_index != other.m_index;
      }

      value_type operator*() const
      {
        const auto& id = m_registry->m_idMap.at(m_index);
        const auto& element = m_registry->m_data.at(m_index);
        return { id, element };
      }

    private:
      size_t m_index;
      const LinearRegistry* m_registry;
    };

    const_iterator begin() const { return const_iterator(0, this); }
    const_iterator end() const { return const_iterator(m_data.size(), this); }

    const_iterator cbegin() const { return const_iterator(0, this); }
    const_iterator cend() const { return const_iterator(m_data.size(), this); }

  private:
    ContainerType m_data;

    std::unordered_map<IdType, size_t> m_indexMap;
    std::unordered_map<size_t, IdType> m_idMap;

    KeySet m_waitingKeys, m_confirmedKeys;

    bool m_isRegistering = false;
    bool m_modified = false;
  };

} // namespace vrm
