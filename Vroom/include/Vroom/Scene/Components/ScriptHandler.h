#pragma once

#include <memory>
#include <utility>

#include "Vroom/Core/Assert.h"
#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Scripting/ScriptComponentPtr.h"
#include "Vroom/Scene/Scripting/ScriptEngine.h"

namespace vrm
{

class ScriptHandler
{
public:

  ScriptHandler() = default;
  ScriptHandler(ScriptComponentPtr&& script) : m_script(std::move(script))
  {}

  ScriptHandler(const ScriptHandler&)            = delete;
  ScriptHandler& operator=(const ScriptHandler&) = delete;
  ScriptHandler(ScriptHandler&&)                 = delete;
  ScriptHandler& operator=(ScriptHandler&&)      = delete;
  virtual ~ScriptHandler()                       = default;

  bool hasScript() const
  {
    return m_script != nullptr;
  }

  ScriptComponent& getScript()
  {
    VRM_ASSERT_MSG(hasScript(), "ScriptHandler has no script assigned.");
    return *m_script;
  }
  const ScriptComponent& getScript() const
  {
    VRM_ASSERT_MSG(hasScript(), "ScriptHandler has no script assigned.");
    return *m_script;
  }

  friend class ScriptEngineAttorney;
  class ScriptEngineAttorney
  {
  public:

    friend ScriptHandler;
    friend class ScriptEngine;

  private:

    ScriptEngineAttorney(ScriptHandler& handler) : handler(handler)
    {}

    void replaceScript(ScriptComponentPtr newScript)
    {
      handler.m_script = std::move(newScript);
      if (handler.m_script != nullptr)
      {
        handler.m_script->onInit();
      }
    }

    ScriptHandler& handler;
  };

  ScriptEngineAttorney getScriptEngineAttorney()
  {
    return ScriptEngineAttorney(*this);
  }

private:

private:

  ScriptComponentPtr m_script;
};

} // namespace vrm
