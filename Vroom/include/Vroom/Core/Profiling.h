#pragma once

#include "Vroom/Core/PerfRecorder.h"
#include "Vroom/Core/Profiler.h"

#define VRM_PROFILE_SCOPE(name)                                     \
  static PerfRecorder s_vrmPerfRecorder    = { name };              \
  ScopePerfRecorder   vrmScopePerfRecorder = { s_vrmPerfRecorder };

namespace vrm
{

class VRM_API ScopePerfRecorder
{
public:

  inline ScopePerfRecorder(PerfRecorder& recorder) : m_recorder(recorder)
  {
    m_recorder.startRecording();
    Profiler::Get().pushRecorder(&recorder);
  }

  inline ~ScopePerfRecorder()
  {
    m_recorder.stopRecording();
    Profiler::Get().popRecorder();
  }

  ScopePerfRecorder() = delete;

  ScopePerfRecorder& operator=(const ScopePerfRecorder& other) = delete;
  ScopePerfRecorder(const ScopePerfRecorder& other)            = delete;

  ScopePerfRecorder& operator=(ScopePerfRecorder&& other) = delete;
  ScopePerfRecorder(ScopePerfRecorder&& other)            = delete;

private:

  PerfRecorder& m_recorder;
};

} // namespace vrm
