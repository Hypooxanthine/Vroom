#include "Vroom/Core/PerfRecorder.h"

#include "Vroom/Core/Duration.h"

using namespace vrm;

PerfRecorder::PerfRecorder(const std::string& name) : m_name(name)
{
  setHistorySize(10);
}

void PerfRecorder::setHistorySize(size_t size)
{
  for (size_t i = size; i < m_history.size(); ++i)
  {
    m_timeSum = m_timeSum - m_history.at(i);
  }

  m_history.resize(size, 0);
  m_historyCursor = std::min(m_historyCursor, size);
  m_sampleCount   = std::min(m_sampleCount, size);

  for (auto& child : m_children) { child->setHistorySize(size); }
}

void PerfRecorder::startRecording() { m_timer.start(); }

void PerfRecorder::stopRecording()
{
  m_timer.stop();

  uint64_t& nanosecs = m_history.at(m_historyCursor);
  m_timeSum          = m_timeSum - nanosecs;
  nanosecs           = m_timer.getNanoseconds();

  m_historyCursor = (m_historyCursor + 1) % m_history.size();
  m_sampleCount   = std::min(m_sampleCount + 1, m_history.size());
}

float PerfRecorder::getMeanTimeSeconds() const
{
  if (m_sampleCount > 0)
    return DurationSeconds(m_timeSum).get<float>()
         / static_cast<float>(m_sampleCount);
  else return 0.f;
}
