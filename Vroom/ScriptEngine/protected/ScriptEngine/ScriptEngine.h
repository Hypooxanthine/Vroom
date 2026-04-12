#pragma once

#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "ScriptEngine/ScriptComponentPtr.h"
#include "ScriptEngine/Api.h"
#include "ScriptEngine/RuntimeScriptLibrary.h"
#include "ScriptEngine/ScriptComponentPtr.h"
#include "ScriptEngine/ScriptHandle.h"

namespace vrm
{

class ScriptFactory;
class ScriptComponent;

class VRM_SCRIPTENGINE_API ScriptEngine
{
public:

  friend ScriptComponentPtr;

public:

  ~ScriptEngine();

  static ScriptEngine& Get();

  bool loadScriptLibrary(const std::filesystem::path& libraryPath);

  inline bool isScriptRegistered(const std::string& scriptId) const
  {
    return m_library->isLoaded() && m_library->scriptExists(scriptId);
  }

  std::vector<std::string> getRegisteredScriptIds() const;

  [[nodiscard]] ScriptComponentPtr createScriptComponent(const std::string& scriptName);

private:

  ScriptEngine();

  ScriptEngine(const ScriptEngine&)            = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;

  ScriptComponent* _getScript(ScriptHandle handle) const;
  void             _destroyScript(ScriptHandle handle);

  std::filesystem::path _copyLibraryFile(const std::filesystem::path& libPath) const;
  bool                  _removeCopiedLibrary() const;
  // bool _checkLibCompatible(const RuntimeScriptLibrary& lib) const;
  // bool _checkScriptsCompatibles(const ScriptHandle& oldScript, const RuntimeScriptLibrary& newLib) const;
  // void _refreshLoadedScriptsUncheck(const RuntimeScriptLibrary& newLib) const;

private:

  static std::unique_ptr<ScriptEngine> s_Instance;

  std::unique_ptr<RuntimeScriptLibrary> m_library;
  uint32_t                              m_libraryGeneration = std::numeric_limits<uint32_t>::max();
  std::filesystem::path                 m_libPath           = {};
};

} // namespace vrm
