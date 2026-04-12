#include "Scene/ScriptHandler.h"

#include "AssetManager/ComponentData.h"

using namespace vrm;

ScriptHandler::ScriptHandler(const ScriptComponentData& data)
{
  if (data.resourceName.empty())
  {
    return;
  }

  VRM_CHECK_MSG(ScriptEngine::Get().isScriptRegistered(data.resourceName), "Script {} is not registered",
                data.resourceName);
  ScriptComponentPtr sc = ScriptEngine::Get().createScriptComponent(data.resourceName);
  m_script              = std::move(sc);
}

ScriptComponentData ScriptHandler::getData() const
{
  ScriptComponentData data;

  if (hasScript())
  {
    data.resourceName = getScript().getScriptName();
  }

  return data;
}
