#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "Vroom/Api.h"
#include "Vroom/Scene/Scripting/RuntimeScriptLibrary.h"
#include "Vroom/Scene/Scripting/ScriptComponentPtr.h"

namespace vrm
{

class ScriptFactory;
class ScriptComponent;

class VRM_API ScriptEngine
{
public:

  ~ScriptEngine();

  static ScriptEngine& Get();

  bool loadScriptLibrary(const std::filesystem::path& libraryPath);

  inline bool isScriptRegistered(const std::string& scriptId) const
  {
    return m_library->scriptExists(scriptId);
  }

  [[nodiscard]] ScriptComponentPtr createScriptComponent(const std::string& scriptName) const;

private:

  ScriptEngine();

  ScriptEngine(const ScriptEngine&)            = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;

  std::filesystem::path _copyLibraryFile(const std::filesystem::path& libPath) const;
  bool _removeCopiedLibrary(const std::filesystem::path& libPath) const;
  bool _checkLibCompatible(const RuntimeScriptLibrary& lib) const;
  bool _checkScriptsCompatibles(const ScriptComponent& oldScript, const RuntimeScriptLibrary& newLib) const;
  void _refreshLoadedScriptsUncheck(const RuntimeScriptLibrary& newLib) const;

private:

  static std::unique_ptr<ScriptEngine> s_Instance;

  std::unique_ptr<RuntimeScriptLibrary> m_library;
  std::filesystem::path m_libPath = {};
};

} // namespace vrm
