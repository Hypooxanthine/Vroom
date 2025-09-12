#pragma once

#include <chrono>

#include "Vroom/Core/Duration.h"
namespace vrm
{

class Timer
{
public:

  inline Timer();

  Timer& operator=(const Timer& other) = delete;
  Timer(const Timer& other)            = delete;

  Timer& operator=(Timer&& other) = delete;
  Timer(Timer&& other)            = delete;

  inline void start();
  inline void stop();

  inline uint64_t getNanoseconds() const { return m_ellapsedTimeNs; }

  template <typename T>
  inline T getSeconds() const
  {
    return DurationSeconds(m_ellapsedTimeNs).get<T>();
  }

private:

  uint64_t                                       m_ellapsedTimeNs;
  std::chrono::high_resolution_clock::time_point m_stamp;
};

inline Timer::Timer() : m_ellapsedTimeNs(0) {}

inline void Timer::start()
{
  m_stamp = std::chrono::high_resolution_clock::now();
}

inline void Timer::stop()
{
  auto now      = std::chrono::high_resolution_clock::now();
  auto duration = now - m_stamp;
  m_ellapsedTimeNs =
    std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

} // namespace vrm
