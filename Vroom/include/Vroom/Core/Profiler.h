#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "Vroom/Api.h"
#include "Vroom/Core/PerfRecorder.h"

namespace vrm
{

class PerfRecorder;

class VRM_API Profiler
{
public:

  ~Profiler();

  Profiler& operator=(const Profiler& other) = delete;
  Profiler(const Profiler& other)            = delete;

  Profiler& operator=(Profiler&& other) = delete;
  Profiler(Profiler&& other)            = delete;

  static void      Init();
  static void      Shutdown();
  static Profiler& Get();

  void newFrame();

  void pushRecorder(const std::string& name);
  void popRecorder();

  const PerfRecorder*              getLastFrameRecorder(size_t id) const;
  std::vector<const PerfRecorder*> getLastFrameRoots() const;

private:

  Profiler();

  inline PerfRecorder& _getRecorder(size_t id) { return m_recorders.at(id); }

private:

  static std::unique_ptr<Profiler> s_instance;

  std::vector<PerfRecorder> m_recorders, m_lastRecorders;
  std::vector<size_t>       m_roots, m_lastRoots;
  std::stack<size_t>        m_stack, m_lastStack;
};

} // namespace vrm
