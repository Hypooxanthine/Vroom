#pragma once

#include <memory>

#include "Vroom/Api.h"

namespace vrm
{

class ScriptComponent;
class RuntimeScriptLibrary;

struct VRM_API ScriptDeleter
{
  const RuntimeScriptLibrary* owner = nullptr;

  void operator()(ScriptComponent* script) const;
};

using ScriptComponentPtr = std::unique_ptr<ScriptComponent, ScriptDeleter>;

} // namespace vrm
