#pragma once

#include <cstddef>

namespace vrm::maths
{

/**
 * @brief Calculates the next multiple of a given number that is greater than or equal to a specified value.
 *
 * This function takes two parameters, `n` and `p`, where `n` is the value to be rounded up 
 * to the nearest multiple of `p`. If `n` is already a multiple of `p`, it returns `n`.
 *
 * @param n The value to be rounded up.
 * @param p The multiple to which `n` should be rounded up.
 * @return The next multiple of `p` that is greater than or equal to `n`.
 */
inline constexpr size_t NextMultipleInclusive(size_t n, size_t p)
{
  const size_t mod = n % p;
  if (mod == 0) return n;
  else return n - mod + p;
}

} // namespace vrm::maths
