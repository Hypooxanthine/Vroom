#pragma once

#include "Vroom/Core/Profiler.h"

#define VRM_PROFILE_SCOPE(name)                     \
  ScopePerfRecorder vrmScopePerfRecorder = { name }

namespace vrm
{

class VRM_API ScopePerfRecorder
{
public:

  inline ScopePerfRecorder(const std::string& name)
  {
    Profiler::Get().pushRecorder(name);
  }

  inline ~ScopePerfRecorder() { Profiler::Get().popRecorder(); }

  ScopePerfRecorder() = delete;

  ScopePerfRecorder& operator=(const ScopePerfRecorder& other) = delete;
  ScopePerfRecorder(const ScopePerfRecorder& other)            = delete;

  ScopePerfRecorder& operator=(ScopePerfRecorder&& other) = delete;
  ScopePerfRecorder(ScopePerfRecorder&& other)            = delete;
};

} // namespace vrm
