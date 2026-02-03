#include "Vroom/Scene/Scripting/RuntimeScriptLibrary.h"

#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Tools/RuntimeLibrary.h"

using namespace vrm;

RuntimeScriptLibrary::RuntimeScriptLibrary()
{}

RuntimeScriptLibrary::~RuntimeScriptLibrary()
{}

bool RuntimeScriptLibrary::load(const std::filesystem::path& libraryPath)
{
  RuntimeLibrary newLib;

  if (!newLib.load(libraryPath))
    return false;

  ScriptConstructor constructor = nullptr;
  if (!(constructor = newLib.getSymbol<ScriptComponent*(size_t)>("VRM_ScriptLibrary_CreateScript")))
    return false;

  ScriptDestructor destructor = nullptr;
  if (!(destructor = newLib.getSymbol<void(ScriptComponent*)>("VRM_ScriptLibrary_DestroyScript")))
    return false;

  ScriptCounter scriptCounter = nullptr;
  if (!(scriptCounter = newLib.getSymbol<size_t()>("VRM_ScriptLibrary_GetScriptCount")))
    return false;

  ScriptNameGetter scriptNameGetter = nullptr;
  if (!(scriptNameGetter = newLib.getSymbol<const char*(size_t)>("VRM_ScriptLibrary_GetScriptName")))
    return false;

  // Loading ok --> we can change class data

  m_scriptConstructor = constructor;
  m_scriptDestructor  = destructor;

  size_t scriptCount = scriptCounter();
  for (size_t i = 0; i < scriptCount; ++i)
  {
    std::string scriptName = scriptNameGetter(i);
    ScriptData& data       = m_scriptsData[scriptName];
    data.name              = std::move(scriptName);
    data.libraryIndex      = i;
  }

  m_runtimeLib = std::move(newLib);

  return true;
}

void RuntimeScriptLibrary::unload()
{
  assert(!isLoaded());

  m_scriptsData.clear();
  m_runtimeLib.unload();
  m_scriptConstructor = nullptr;
  m_scriptDestructor  = nullptr;
}

ScriptComponentPtr RuntimeScriptLibrary::createScript(const std::string& scriptName) const
{
  if (m_scriptConstructor != nullptr)
  {
    if (auto it = m_scriptsData.find(scriptName); it != m_scriptsData.end())
    {
      ScriptComponent* rawPtr = m_scriptConstructor(it->second.libraryIndex);
      return ScriptComponentPtr(rawPtr, ScriptDeleter{ this });
    }
  }

  return {};
}

void RuntimeScriptLibrary::_destroyScript(ScriptComponent* script) const
{
  if (m_scriptDestructor != nullptr && script != nullptr)
  {
    m_scriptDestructor(script);
  }
}
