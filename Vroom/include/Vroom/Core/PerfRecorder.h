#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
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

  PerfRecorder& operator=(PerfRecorder&& other) = default;
  PerfRecorder(PerfRecorder&& other)            = default;

  inline const std::string& getName() const { return m_name; }

  void startRecording();
  void stopRecording();

  uint64_t getStartTimeNanosecs() const;
  uint64_t getEndTimeNanosecs() const;
  double   getTimeSeconds() const;

  inline size_t                    getId() const { return m_id; }
  std::vector<const PerfRecorder*> getChildren() const;
  const PerfRecorder*              getParent() const;

  inline void setId(size_t id) { m_id = id; }
  inline void addChild(size_t child) { m_children.push_back(child); }
  inline void setParent(size_t parent) { m_parent = parent; }

private:

  std::string m_name;
  Timer       m_timer;

  size_t              m_id = std::numeric_limits<size_t>::max();
  std::vector<size_t> m_children;
  size_t              m_parent = std::numeric_limits<size_t>::max();
};

} // namespace vrm
