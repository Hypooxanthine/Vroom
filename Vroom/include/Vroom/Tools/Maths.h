#pragma once

#include <cstddef>

namespace vrm::maths
{

/**
 * @brief If n is a power of 16, returns n. Returns next multiple of power of 16
 * otherwise.
 */
inline static constexpr size_t NextPowerMultipleInclusive(size_t n, size_t pow)
{
  const size_t mod = n % pow;
  if (mod == 0) return n;
  else return n - mod + pow;
}

} // namespace vrm::maths
