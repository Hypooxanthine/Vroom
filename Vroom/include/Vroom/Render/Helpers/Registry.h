#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Vroom/Core/Assert.h"

namespace vrm
{

  template <typename T, typename IdType>
  class Registry
  {
  public:
    using ContainerType = std::unordered_map<IdType, T>;
    using iterator = ContainerType::iterator;
    using const_iterator = ContainerType::const_iterator;

  public:

    inline Registry()
    {

    }

    inline ~Registry()
    {

    }

    inline void reset()
    {
      m_isRegistering = false;
      m_data.clear();
      m_waitingKeys.clear();
      m_confirmedKeys.clear();
    }

    inline bool contains(const IdType& id) const
    {
      return m_data.contains(id);
    }

    inline void startRegistering()
    {
      VRM_ASSERT_MSG(m_isRegistering == false, "Already registering");
      m_isRegistering = true;

      std::swap(m_waitingKeys, m_confirmedKeys);

      m_waitingKeys.clear();
    }

    inline void endRegistering()
    {
      VRM_ASSERT_MSG(m_isRegistering == true, "Wasn't registering");
      m_isRegistering = false;

      for (const auto& missingId : m_waitingKeys)
      {
        m_data.erase(missingId);
      }
    }

    template <typename uT>
      requires std::is_same_v<std::decay_t<uT>, T>
    inline void submit(const IdType& id, uT&& element)
    {
      notifyUsed(id);
      m_data.insert_or_assign(id, std::forward<uT>(element));
    }

    inline void notifyUsed(const IdType& id)
    {
      VRM_ASSERT_MSG(m_isRegistering == true, "Wasn't registering");
      m_waitingKeys.erase(id);
      m_confirmedKeys.insert(id);
    }

    inline iterator begin() { return m_data.begin(); }
    inline const_iterator begin() const { return m_data.begin(); }
    inline const_iterator cbegin() const { return m_data.cbegin(); }

    inline iterator end() { return m_data.end(); }
    inline const_iterator end() const { return m_data.end(); }
    inline const_iterator cend() const { return m_data.cend(); }

  private:
    ContainerType m_data;

    std::unordered_set<IdType> m_waitingKeys, m_confirmedKeys;

    bool m_isRegistering = false;
  };

} // namespace vrm
