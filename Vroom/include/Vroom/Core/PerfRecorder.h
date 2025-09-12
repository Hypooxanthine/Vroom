#pragma once

#include <string>
#include <vector>

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

  float getMeanTimeSeconds() const;

  inline std::span<PerfRecorder* const> getChildren() const
  {
    return m_children;
  }

  inline void addChild(PerfRecorder* child) { m_children.push_back(child); }

private:

  std::string m_name;
  Timer       m_timer;

  std::vector<uint64_t> m_history;
  size_t                m_historyCursor = 0;
  uint64_t              m_timeSum       = 0;
  size_t                m_sampleCount   = 0;

  std::vector<PerfRecorder*> m_children;
};

} // namespace vrm
