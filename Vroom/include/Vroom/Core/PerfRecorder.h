#pragma once

#include <string>
#include <vector>

#include "Vroom/Api.h"
#include "Vroom/Core/Timer.h"

namespace vrm
{

class VRM_API PerfRecorder
{
public:

  PerfRecorder(const std::string& name);

  PerfRecorder& operator=(const PerfRecorder& other) = delete;
  PerfRecorder(const PerfRecorder& other)            = delete;

  PerfRecorder& operator=(PerfRecorder&& other) = delete;
  PerfRecorder(PerfRecorder&& other)            = delete;

  inline const std::string& getName() const { return m_name; }

  void setHistorySize(size_t size);

  void startRecording();
  void stopRecording();

  double   getMeanTimeSeconds() const;
  uint64_t getStartTimeNanosecs() const;

  inline std::span<PerfRecorder* const> getChildren() const
  {
    return m_children;
  }

  inline PerfRecorder* getParent() const { return m_parent; }

  inline void addChild(PerfRecorder* child) { m_children.push_back(child); }
  inline void setParent(PerfRecorder* parent) { m_parent = parent; }

private:

  std::string          m_name;
  Timer                m_timer;
  Timer::TimeStampType m_previousStart, m_previousEnd;

  std::vector<uint64_t> m_history;
  size_t                m_historyCursor = 0;
  uint64_t              m_timeSum       = 0;
  size_t                m_sampleCount   = 0;

  std::vector<PerfRecorder*> m_children;
  PerfRecorder*              m_parent = nullptr;
};

} // namespace vrm
