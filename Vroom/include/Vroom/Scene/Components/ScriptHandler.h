#pragma once

#include <memory>

#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Scripting/ScriptComponentPtr.h"

namespace vrm
{

class ScriptHandler
{
public:

  ScriptHandler(ScriptComponentPtr&& script) : m_Script(std::move(script))
  {}

  ScriptHandler(const ScriptHandler&)            = delete;
  ScriptHandler& operator=(const ScriptHandler&) = delete;
  ScriptHandler(ScriptHandler&&)                 = delete;
  ScriptHandler& operator=(ScriptHandler&&)      = delete;
  virtual ~ScriptHandler()                       = default;

  ScriptComponent& getScript()
  {
    return *m_Script;
  }
  const ScriptComponent& getScript() const
  {
    return *m_Script;
  }

private:

  ScriptComponentPtr m_Script;
};

} // namespace vrm
