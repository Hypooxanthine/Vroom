#pragma once

#include <cstddef>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "ScriptEngine/Api.h"
#include "ScriptEngine/ScriptHandle.h"
#include "Tools/RuntimeLibrary.h"


namespace vrm
{

class ScriptComponent;

class VRM_SCRIPTENGINE_API RuntimeScriptLibrary
{
public:

  RuntimeScriptLibrary();
  ~RuntimeScriptLibrary();

  RuntimeScriptLibrary(const RuntimeScriptLibrary& other)            = delete;
  RuntimeScriptLibrary& operator=(const RuntimeScriptLibrary& other) = delete;

  // Enabling these constructors is very dangerous since ScriptPtr deleter holds a ptr to
  // the RuntimeScriptLibrary itself. It would need to change the deleter behaviour by
  // referencing the destructor function ptr instead for example, but it would disable
  // the RuntimeScriptLibrary from keeping a count on alive entities.
  RuntimeScriptLibrary(RuntimeScriptLibrary&& other)            = delete;
  RuntimeScriptLibrary& operator=(RuntimeScriptLibrary&& other) = delete;

  inline bool isLoaded() const
  {
    return m_runtimeLib.isLoaded();
  }

  /**
   * @brief
   *
   * @param libraryPath
   * @return true
   * @return false
   */
  bool load(const std::filesystem::path& libraryPath);
  void unload();

  /**
   * @brief Create a new script from the loaded script library.
   *
   */
  ScriptHandle createScript(const std::string& scriptName);

  ScriptComponent* getScript(ScriptHandle handle) const;

  void destroyScript(ScriptHandle handle);

  inline bool scriptExists(const std::string& scriptName) const
  {
    return isLoaded() && m_scriptsData.contains(scriptName);
  }

  std::vector<std::string> getScriptNames() const;

private:

  using ScriptConstructor = ScriptComponent* (*)(size_t);
  using ScriptDestructor  = void (*)(ScriptComponent*);
  using ScriptCounter     = size_t (*)();
  using ScriptNameGetter  = const char* (*)(size_t);

  struct _ScriptData
  {
    std::string name;
    size_t      libraryIndex;
  };

  struct _ScriptSlot
  {
    ScriptComponent* script     = nullptr;
    size_t           generation = 0;
  };

private:

  void _destroyScript(ScriptComponent* script) const;

private:

  RuntimeLibrary m_runtimeLib;

  ScriptConstructor m_scriptConstructor = nullptr;
  ScriptDestructor  m_scriptDestructor  = nullptr;

  std::unordered_map<std::string, _ScriptData> m_scriptsData;

  std::vector<_ScriptSlot> m_scripts;
  std::vector<size_t>      m_freeScriptIdx;
};

} // namespace vrm
