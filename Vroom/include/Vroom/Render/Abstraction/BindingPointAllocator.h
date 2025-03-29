#pragma once

#include <set>
#include <limits>

namespace vrm::gl
{

  template <typename T>
  class BindingPointAllocator
  {
  public:
    inline static constexpr T Invalid = std::numeric_limits<T>::max();
  public:
    inline constexpr BindingPointAllocator(const T& firstAvailable, size_t count) :
      m_maxBindingPointCount(count)
    {
      T element = firstAvailable;
      for (size_t i = 0; i < count; ++i)
      {
        m_availablePoints.emplace(element);
        ++element;
      }
    }

    inline ~BindingPointAllocator() {}

    BindingPointAllocator(const BindingPointAllocator&) = delete;
    BindingPointAllocator& operator=(const BindingPointAllocator&) = delete;
    BindingPointAllocator(BindingPointAllocator&&) = delete;
    BindingPointAllocator& operator=(BindingPointAllocator&&) = delete;

    T next()
    {
      if (m_availablePoints.empty())
        return Invalid;

      auto nextIt = m_availablePoints.begin();
      T out = *nextIt;
      m_availablePoints.erase(nextIt);

      return out;
    }

    void giveBack(const T& element)
    {
      m_availablePoints.emplace(element);
    }

    inline size_t getMaxBindingPointCount() const { return m_maxBindingPointCount; }

  private:
    std::set<T> m_availablePoints;
    size_t m_maxBindingPointCount;
  };

} // namespace vrm::gl
