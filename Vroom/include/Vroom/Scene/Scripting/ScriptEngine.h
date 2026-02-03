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
    return m_library.scriptExists(scriptId);
  }

  [[nodiscard]] ScriptComponentPtr createScriptComponent(const std::string& scriptName) const;

private:

private:

  ScriptEngine();

  ScriptEngine(const ScriptEngine&)            = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;

private:

  static std::unique_ptr<ScriptEngine> s_Instance;

  RuntimeScriptLibrary m_library;
};

} // namespace vrm
