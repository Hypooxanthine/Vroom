#pragma once

#include <cstdint>

#include "Vroom/Api.h"

namespace vrm
{

template <uint64_t Divisor>
class VRM_API Duration
{
public:

  constexpr Duration(uint64_t nanosecs) : m_nanoseconds(nanosecs) {}

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

  uint64_t m_nanoseconds;
};

/* Template helpers */

inline const constinit uint64_t DURATION_DIV_SECONDS      = 1'000'000'000;
inline const constinit uint64_t DURATION_DIV_MILLISECONDS = 1'000'000;
inline const constinit uint64_t DURATION_DIV_MICROSECONDS = 1'000;
inline const constinit uint64_t DURATION_DIV_NANOSECONDS  = 1;

using DurationSeconds      = Duration<DURATION_DIV_SECONDS>;
using DurationMilliseconds = Duration<DURATION_DIV_MILLISECONDS>;
using DurationMicroseconds = Duration<DURATION_DIV_MICROSECONDS>;
using DurationNanoseconds  = Duration<DURATION_DIV_NANOSECONDS>;

/* PLUS */

template <uint64_t D, typename T>
inline constexpr T operator+(const T& lhs, const Duration<D>& rhs)
{
  return lhs + rhs.template get<T>();
}

template <uint64_t D, typename T>
inline constexpr T operator+(const Duration<D>& lhs, const T& rhs)
{
  return lhs.template get<T>() + rhs;
}

template <uint64_t D, typename T>
inline constexpr T& operator+=(T& lhs, const Duration<D>& rhs)
{
  lhs = lhs + rhs.template get<T>();
  return lhs;
}

/* MINUS */

template <uint64_t D, typename T>
inline constexpr T operator-(const T& lhs, const Duration<D>& rhs)
{
  return lhs - rhs.template get<T>();
}

template <uint64_t D, typename T>
inline constexpr T operator-(const Duration<D>& lhs, const T& rhs)
{
  return lhs.template get<T>() - rhs;
}

template <uint64_t D, typename T>
inline constexpr T& operator-=(T& lhs, const Duration<D>& rhs)
{
  lhs = lhs - rhs.template get<T>();
  return lhs;
}

/* MULT */

template <uint64_t D, typename T>
inline constexpr T operator*(const T& lhs, const Duration<D>& rhs)
{
  return lhs * rhs.template get<T>();
}

template <uint64_t D, typename T>
inline constexpr T operator*(const Duration<D>& lhs, const T& rhs)
{
  return lhs.template get<T>() * rhs;
}

template <uint64_t D, typename T>
inline constexpr T& operator*=(T& lhs, const Duration<D>& rhs)
{
  lhs = lhs * rhs.template get<T>();
  return lhs;
}

/* DIV */

template <uint64_t D, typename T>
inline constexpr T operator/(const T& lhs, const Duration<D>& rhs)
{
  return lhs / rhs.template get<T>();
}

template <uint64_t D, typename T>
inline constexpr T operator/(const Duration<D>& lhs, const T& rhs)
{
  return lhs.template get<T>() / rhs;
}

template <uint64_t D, typename T>
inline constexpr T& operator/=(T& lhs, const Duration<D>& rhs)
{
  lhs = lhs / rhs.template get<T>();
  return lhs;
}

} // namespace vrm
