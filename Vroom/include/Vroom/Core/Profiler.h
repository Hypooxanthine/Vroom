#pragma once

#include <memory>
#include <span>
#include <stack>
#include <vector>

#include "Vroom/Api.h"

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

  void notifyRecorderCreation(PerfRecorder* newRecorder);

  void pushRecorder(PerfRecorder* recorder);
  void popRecorder();

  PerfRecorder* getCurrentRecorder() const
  {
    return m_stack.empty() ? nullptr : m_stack.top();
  }

  inline std::span<PerfRecorder* const> getRoots() const { return m_roots; }

private:

  Profiler();

private:

  static std::unique_ptr<Profiler> s_instance;

  std::vector<PerfRecorder*> m_roots;
  std::stack<PerfRecorder*>  m_stack;
};

} // namespace vrm
