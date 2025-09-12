#include "Vroom/Core/Profiler.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Core/PerfRecorder.h"

using namespace vrm;

std::unique_ptr<Profiler> Profiler::s_instance;

Profiler::Profiler() {}

Profiler::~Profiler() {}

void Profiler::Init() { s_instance.reset(new Profiler()); }

void Profiler::Shutdown() { s_instance.reset(); }

Profiler& Profiler::Get()
{
  VRM_DEBUG_ASSERT_MSG(
    s_instance, "Profiler not initialized. Call Profiler::Init() first.");

  return *s_instance;
}

void Profiler::notifyRecorderCreation(PerfRecorder* newRecorder)
{
  if (m_stack.empty())
  {
    // No other recorder is active : new root
    m_roots.push_back(newRecorder);
  }
  else
  {
    // Top of the stack has a new child
    m_stack.top()->addChild(newRecorder);
  }
}

void Profiler::pushRecorder(PerfRecorder* recorder) { m_stack.push(recorder); }

void Profiler::popRecorder() { m_stack.pop(); }
