#include "ScriptEngine/RuntimeScriptLibrary.h"

#include <algorithm>
#include <cassert>

#include "ScriptEngine/ScriptHandle.h"
#include "Tools/RuntimeLibrary.h"

using namespace vrm;

RuntimeScriptLibrary::RuntimeScriptLibrary()
{}

RuntimeScriptLibrary::~RuntimeScriptLibrary()
{
  if (isLoaded())
    unload();
}

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
    _ScriptData& data       = m_scriptsData[scriptName];
    data.name              = std::move(scriptName);
    data.libraryIndex      = i;
  }

  m_runtimeLib = std::move(newLib);

  return true;
}

void RuntimeScriptLibrary::unload()
{
  assert(isLoaded());

  for (_ScriptSlot& slot : m_scripts)
  {
    _destroyScript(slot.script);
  }

  m_scripts.clear();
  m_freeScriptIdx.clear();
  m_scriptsData.clear();
  m_runtimeLib.unload();
  m_scriptConstructor = nullptr;
  m_scriptDestructor  = nullptr;
}

ScriptHandle RuntimeScriptLibrary::createScript(const std::string& scriptName)
{
  ScriptHandle handle;

  if (m_scriptConstructor != nullptr)
  {
    if (auto it = m_scriptsData.find(scriptName); it != m_scriptsData.end())
    {
      ScriptComponent* rawPtr = m_scriptConstructor(it->second.libraryIndex);
      if (rawPtr)
      {
        size_t index;
        if (m_freeScriptIdx.empty())
        {
          index = m_scripts.size();
          m_scripts.emplace_back();
        }
        else
        {
          index = m_freeScriptIdx.back();
          m_freeScriptIdx.pop_back();
        }

        _ScriptSlot& slot = m_scripts.at(index);
        slot.script = rawPtr;
        // slot.generation is 0 on creation of the slot
        // and incremented when the slot's script is destroyed:
        // when we retrieve an existing slot, it is already incremented

        handle.setId(index);
        handle.setGeneration(slot.generation);
      }
    }
  }

  return handle;
}

ScriptComponent* RuntimeScriptLibrary::getScript(ScriptHandle handle) const
{
  ScriptComponent* script = nullptr;

  auto index = handle.getId();
  if (index < m_scripts.size())
  {
    const _ScriptSlot& slot = m_scripts.at(index);

    if (slot.generation == handle.getGeneration())
    {
      script = slot.script;
    }
  }

  return script;
}

void RuntimeScriptLibrary::destroyScript(ScriptHandle handle)
{
  auto index = handle.getId();

  if (index < m_scripts.size())
  {
    _ScriptSlot& slot = m_scripts.at(index);

    if (slot.generation == handle.getGeneration())
    {
      _destroyScript(slot.script);

      slot.script = nullptr;
      ++slot.generation;

      m_freeScriptIdx.emplace_back(index);
    }
  }
}

void RuntimeScriptLibrary::_destroyScript(ScriptComponent* script) const
{
  if (m_scriptDestructor != nullptr && script != nullptr)
  {
    m_scriptDestructor(script);
  }
}

std::vector<std::string> RuntimeScriptLibrary::getScriptNames() const
{
  std::vector<std::string> names;
  names.reserve(m_scriptsData.size());

  for (const auto& [scriptName, data] : m_scriptsData)
  {
    names.emplace_back(scriptName);
  }

  std::sort(names.begin(), names.end());
  return names;
}
