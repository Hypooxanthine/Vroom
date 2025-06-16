#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Vroom/Api.h"

namespace vrm
{

#define VRM_GEN_SCRIPT_ID(ScriptClass) "VRM_" #ScriptClass "_ID"

class ScriptFactory;
class ScriptComponent;

class VRM_API ScriptEngine
{
public:
  ~ScriptEngine();

  static ScriptEngine& Get();

  inline bool isScriptRegistered(const std::string& scriptId) const { return m_Factories.contains(scriptId); }

  void registerScript(const std::string& scriptId, std::unique_ptr<ScriptFactory>&& factory);
  void unregisterScript(const std::string& scriptId);

  [[nodiscard]] ScriptComponent* createScriptComponent(const std::string& scriptId) const;

private:
  ScriptEngine();

  ScriptEngine(const ScriptEngine&) = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;
  
  static std::unique_ptr<ScriptEngine> s_Instance;
  std::unordered_map<std::string, std::unique_ptr<ScriptFactory>> m_Factories;
};

} // namespace vrm
