#include "Vroom/Core/Profiler.h"
#include <vector>

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

void Profiler::newFrame()
{
  std::swap(m_recorders, m_lastRecorders);
  std::swap(m_stack, m_lastStack);
  std::swap(m_roots, m_lastRoots);

  m_recorders.clear();
  m_stack = {};
  m_roots.clear();
}

void Profiler::pushRecorder(const std::string& name)
{
  size_t        id       = m_recorders.size();
  PerfRecorder& recorder = m_recorders.emplace_back(name);
  recorder.setId(id);
  recorder.startRecording();

  if (m_stack.empty())
  {
    // No other recorder is active : new root
    m_roots.push_back(id);
  }
  else
  {
    // Top of the stack has a new child
    size_t        parentId = m_stack.top();
    PerfRecorder& parent   = _getRecorder(parentId);
    parent.addChild(id);
    recorder.setParent(parentId);
  }

  m_stack.push(id);
}

void Profiler::popRecorder()
{
  _getRecorder(m_stack.top()).stopRecording();
  m_stack.pop();
}

const PerfRecorder* Profiler::getLastFrameRecorder(size_t id) const
{
  if (id < m_lastRecorders.size()) { return &m_lastRecorders.at(id); }
  else return nullptr;
}

std::vector<const PerfRecorder*> Profiler::getLastFrameRoots() const
{
  std::vector<const PerfRecorder*> roots;
  roots.reserve(m_lastRoots.size());

  for (size_t rootId : m_lastRoots)
  {
    roots.emplace_back(&m_lastRecorders.at(rootId));
  }

  return roots;
}
