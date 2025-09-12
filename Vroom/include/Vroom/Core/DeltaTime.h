#pragma once

#include <chrono>

#include "Vroom/Api.h"
#include "Vroom/Core/Duration.h"

namespace vrm
{

class VRM_API DeltaTime
{
public:

  using ClockType     = std::chrono::high_resolution_clock;
  using TimePointType = ClockType::time_point;

  using DurationType = ClockType::duration;

public:

  inline constexpr DeltaTime(const TimePointType& internalFrom,
                             const DurationType&  internalDuration)
    : m_stamp(internalFrom), m_duration(internalDuration)
  {}

  inline DeltaTime() : DeltaTime(ClockType::now(), DurationType(0)) {}

  inline void next()
  {
    auto now   = ClockType::now();
    m_duration = now - m_stamp;
    m_stamp    = now;
  }

  inline constexpr DurationNanoseconds nanoseconds() const
  {
    return { getNanosecondsCounts() };
  }

  inline constexpr DurationMicroseconds microseconds() const
  {
    return { getNanosecondsCounts() };
  }

  inline constexpr DurationMilliseconds milliseconds() const
  {
    return { getNanosecondsCounts() };
  }

  inline constexpr DurationSeconds seconds() const
  {
    return { getNanosecondsCounts() };
  }

private:

  inline constexpr uint64_t getNanosecondsCounts() const
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration)
      .count();
  }

private:

  TimePointType m_stamp;
  DurationType  m_duration;
};

} // namespace vrm
