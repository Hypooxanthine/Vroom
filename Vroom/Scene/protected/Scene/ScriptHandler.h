#pragma once

#include <utility>

#include "AssetManager/ComponentDataFwds.h"
#include "Core/Assert.h"
#include "Scene/ScriptComponent.h"
#include "ScriptEngine/ScriptComponentPtr.h"
#include "ScriptEngine/ScriptEngine.h"

namespace vrm
{

class VRM_SCENE_API ScriptHandler
{
public:

  ScriptHandler() = default;
  explicit ScriptHandler(ScriptComponentPtr&& script) : m_script(std::move(script))
  {}

  explicit ScriptHandler(const ScriptComponentData& data);

  ScriptHandler(const ScriptHandler&)            = delete;
  ScriptHandler& operator=(const ScriptHandler&) = delete;
  ScriptHandler(ScriptHandler&&)                 = delete;
  ScriptHandler& operator=(ScriptHandler&&)      = delete;
  virtual ~ScriptHandler()                       = default;

  ScriptComponentData getData() const;

  bool hasScript() const
  {
    return m_script.isValid();
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
      if (handler.m_script.isValid())
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
