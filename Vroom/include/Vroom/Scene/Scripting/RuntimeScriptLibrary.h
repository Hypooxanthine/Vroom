#pragma once

#include <cstddef>
#include <filesystem>
#include <unordered_map>

#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Scripting/ScriptComponentPtr.h"
#include "Vroom/Tools/RuntimeLibrary.h"

namespace vrm
{

class RuntimeScriptLibrary
{
public:

  friend struct ScriptDeleter;

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
   * @param scriptName
   * @return ScriptComponentPtr The created script, or nullptr if scriptName does not name a script in the library.
   */
  ScriptComponentPtr createScript(const std::string& scriptName) const;

  inline bool scriptExists(const std::string& scriptName) const
  {
    assert(isLoaded());
    return m_scriptsData.contains(scriptName);
  }

private:

  using ScriptConstructor = ScriptComponent* (*)(size_t);
  using ScriptDestructor  = void (*)(ScriptComponent*);
  using ScriptCounter     = size_t (*)();
  using ScriptNameGetter  = const char* (*)(size_t);

  struct ScriptData
  {
    std::string name;
    size_t      libraryIndex;
  };

private:

  void _destroyScript(ScriptComponent* script) const;

private:

  RuntimeLibrary m_runtimeLib;

  ScriptConstructor m_scriptConstructor = nullptr;
  ScriptDestructor  m_scriptDestructor  = nullptr;

  std::unordered_map<std::string, ScriptData> m_scriptsData;
  mutable size_t                              m_instanceCount = 0;
};

} // namespace vrm
