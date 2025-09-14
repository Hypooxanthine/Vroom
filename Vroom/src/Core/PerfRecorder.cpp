#include "Vroom/Core/PerfRecorder.h"
#include <chrono>
#include <cstddef>

#include "Vroom/Core/Duration.h"
#include "Vroom/Core/Profiler.h"

using namespace vrm;

PerfRecorder::PerfRecorder(const std::string& name) : m_name(name) {}

void PerfRecorder::startRecording() { m_timer.start(); }

void PerfRecorder::stopRecording() { m_timer.stop(); }

double PerfRecorder::getTimeSeconds() const
{
  if (m_timer.isStarted()) return 0.0;
  else return m_timer.getLastDuration<DurationSeconds>().get<double>();
}

uint64_t PerfRecorder::getStartTimeNanosecs() const
{
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           m_timer.getStartStamp().time_since_epoch())
    .count();
}

uint64_t PerfRecorder::getEndTimeNanosecs() const
{
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           m_timer.getEndStamp().time_since_epoch())
    .count();
}

std::vector<const PerfRecorder*> PerfRecorder::getChildren() const
{
  std::vector<const PerfRecorder*> children;
  children.reserve(m_children.size());

  Profiler& profiler = Profiler::Get();

  for (size_t childId : m_children)
  {
    children.push_back(profiler.getLastFrameRecorder(childId));
  }

  return children;
}

const PerfRecorder* PerfRecorder::getParent() const
{
  return Profiler::Get().getLastFrameRecorder(m_parent);
}
