#include "Vroom/Scene/Scripting/ScriptEngine.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Scene/Components/ScriptComponent.h"

using namespace vrm;

std::unique_ptr<ScriptEngine> ScriptEngine::s_Instance = nullptr;

ScriptEngine::ScriptEngine()
{}

ScriptEngine::~ScriptEngine()
{}

ScriptEngine& ScriptEngine::Get()
{
  if (!s_Instance)
  {
    [[unlikely]] s_Instance.reset(new ScriptEngine());
  }

  return *s_Instance;
}

bool ScriptEngine::loadScriptLibrary(const std::filesystem::path& libraryPath)
{
  return m_library.load(libraryPath);
}

ScriptComponentPtr ScriptEngine::createScriptComponent(const std::string& scriptName) const
{
  VRM_ASSERT_MSG(m_library.isLoaded(), "No pointer to the script constructor. Is the librairy loaded ?");
  VRM_ASSERT_MSG(m_library.scriptExists(scriptName), "Could not find {}", scriptName);

  ScriptComponentPtr script = m_library.createScript(scriptName);
  script->m_scriptName = scriptName;
  return script;
}
