#pragma once

#include <chrono>
#include <cstdint>

namespace vrm
{

class Timer
{
public:

  using TimeStampType = std::chrono::high_resolution_clock::time_point;

public:

  inline Timer();

  Timer& operator=(const Timer& other) = delete;
  Timer(const Timer& other)            = delete;

  Timer& operator=(Timer&& other) = default;
  Timer(Timer&& other)            = default;

  inline void start();
  inline void stop();
  inline void restart();

  inline bool isStarted() const { return m_started; }

  inline const auto& getStartStamp() const { return m_start; }
  inline const auto& getEndStamp() const { return m_end; }

  inline uint64_t getLastNanoseconds() const { return m_ellapsedTimeNs; }

  template <typename DurationType>
  inline DurationType getLastDuration() const
  {
    return DurationType(m_ellapsedTimeNs);
  }

  template <typename DurationType>
  inline DurationType getCurrentDuration()
  {
    const TimeStampType now = std::chrono::high_resolution_clock::now();
    const uint64_t      nanoSecs =
      std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start)
        .count();
    return DurationType(nanoSecs);
  }

private:

  bool          m_started = false;
  uint64_t      m_ellapsedTimeNs;
  TimeStampType m_start, m_end;
};

inline Timer::Timer() : m_ellapsedTimeNs(0) {}

inline void Timer::start()
{
  if (m_started == false)
  {
    m_start   = std::chrono::high_resolution_clock::now();
    m_started = true;
  }
}

inline void Timer::stop()
{
  if (m_started == true)
  {
    m_end         = std::chrono::high_resolution_clock::now();
    auto duration = m_end - m_start;
    m_ellapsedTimeNs =
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    m_started = false;
  }
}

inline void Timer::restart()
{
  if (m_started) stop();
  start();
}

} // namespace vrm
