#include "Vroom/Scene/Scripting/ScriptEngine.h"

#include "Vroom/Core/Assert.h"

#include "Vroom/Scene/Scripting/ScriptFactory.h"
#include "Vroom/Scene/Components/ScriptComponent.h"

using namespace vrm;

std::unique_ptr<ScriptEngine> ScriptEngine::s_Instance = nullptr;

ScriptEngine::ScriptEngine()
{

}

ScriptEngine::~ScriptEngine()
{

}

ScriptEngine& ScriptEngine::Get()
{
  if (!s_Instance)
  {
    [[unlikely]]
    s_Instance.reset(new ScriptEngine());
  }

  return *s_Instance;
}

void ScriptEngine::registerScript(const std::string& scriptId, std::unique_ptr<ScriptFactory>&& factory)
{
  VRM_ASSERT_MSG(!isScriptRegistered(scriptId), "Script already registered");

  m_Factories[scriptId] = std::move(factory);
}

void ScriptEngine::unregisterScript(const std::string& scriptId)
{
  VRM_ASSERT_MSG(isScriptRegistered(scriptId), "Script is not registered");

  m_Factories.erase(scriptId);
}

ScriptComponent* ScriptEngine::createScriptComponent(const std::string& scriptId) const
{
  auto* script = m_Factories.at(scriptId)->create();
  script->m_scriptName = scriptId;

  return script;
}
