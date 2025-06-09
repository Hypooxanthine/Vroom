#pragma once

#include <chrono>
#include <utility>

namespace vrm
{

  class DeltaTime
  {
  public:

    using ClockType = std::chrono::high_resolution_clock;
    using TimePointType = ClockType::time_point;

    using DurationType = ClockType::duration;

    template <uint64_t Divisor>
    class Duration
    {
      friend DeltaTime;

    public:

      template <typename T>
      inline constexpr T get() const
      {
        return static_cast<T>(m_nanoseconds) / static_cast<T>(Divisor);
      }

      template <typename T>
      inline constexpr operator T() const
      {
        return get<T>();
      }

    private:
      constexpr Duration(uint64_t nanosecs)
        : m_nanoseconds(nanosecs)
        {}

      uint64_t m_nanoseconds;
    };

  public:

    inline constexpr DeltaTime(const TimePointType& internalFrom, const DurationType& internalDuration)
      : m_stamp(internalFrom), m_duration(internalDuration)
    {
    }

    inline DeltaTime()
      : DeltaTime(ClockType::now(), DurationType(0))
    {
    }

    inline void next()
    {
      auto now = ClockType::now();
      m_duration = now - m_stamp;
      m_stamp = now;
    }

    inline constexpr Duration<1> nanoseconds() const
    {
      return { getNanosecondsCounts() };
    }

    inline constexpr Duration<1'000> microseconds() const
    {
      return { getNanosecondsCounts() };
    }

    inline constexpr Duration<1'000'000> milliseconds() const
    {
      return { getNanosecondsCounts() };
    }

    inline constexpr Duration<1'000'000'000> seconds() const
    {
      return { getNanosecondsCounts() };
    }

  private:

  inline constexpr uint64_t getNanosecondsCounts() const
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration).count();
  }

  private:
    TimePointType m_stamp;
    DurationType m_duration;
  };

  /* PLUS */

  template <uint64_t D, typename T>
  inline constexpr T operator+(const T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    return lhs + rhs.template get<T>();
  }

  template <uint64_t D, typename T>
  inline constexpr T operator+(const DeltaTime::Duration<D>& lhs, const T& rhs)
  {
    return lhs.template get<T>() + rhs;
  }

  template <uint64_t D, typename T>
  inline constexpr T& operator+=(T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    lhs = lhs + rhs.template get<T>();
    return lhs;
  }

  /* MINUS */

  template <uint64_t D, typename T>
  inline constexpr T operator-(const T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    return lhs - rhs.template get<T>();
  }

  template <uint64_t D, typename T>
  inline constexpr T operator-(const DeltaTime::Duration<D>& lhs, const T& rhs)
  {
    return lhs.template get<T>() - rhs;
  }

  template <uint64_t D, typename T>
  inline constexpr T& operator-=(T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    lhs = lhs - rhs.template get<T>();
    return lhs;
  }

  /* MULT */

  template <uint64_t D, typename T>
  inline constexpr T operator*(const T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    return lhs * rhs.template get<T>();
  }

  template <uint64_t D, typename T>
  inline constexpr T operator*(const DeltaTime::Duration<D>& lhs, const T& rhs)
  {
    return lhs.template get<T>() * rhs;
  }

  template <uint64_t D, typename T>
  inline constexpr T& operator*=(T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    lhs = lhs * rhs.template get<T>();
    return lhs;
  }

  /* DIV */

  template <uint64_t D, typename T>
  inline constexpr T operator/(const T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    return lhs / rhs.template get<T>();
  }

  template <uint64_t D, typename T>
  inline constexpr T operator/(const DeltaTime::Duration<D>& lhs, const T& rhs)
  {
    return lhs.template get<T>() / rhs;
  }

  template <uint64_t D, typename T>
  inline constexpr T& operator/=(T& lhs, const DeltaTime::Duration<D>& rhs)
  {
    lhs = lhs / rhs.template get<T>();
    return lhs;
  }

}