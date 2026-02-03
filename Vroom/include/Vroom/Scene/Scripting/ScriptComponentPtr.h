#pragma once

#include <memory>

namespace vrm
{

class ScriptComponent;
class RuntimeScriptLibrary;

struct ScriptDeleter
{
  const RuntimeScriptLibrary* owner = nullptr;

  void operator()(ScriptComponent* script) const;
};

using ScriptComponentPtr = std::unique_ptr<ScriptComponent, ScriptDeleter>;

} // namespace vrm
