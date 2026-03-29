#include "Vroom/Asset/StaticAsset/ScriptAsset.h"

#include "Vroom/Scene/Scripting/ScriptEngine.h"

using namespace vrm;

ScriptAsset::ScriptAsset() : StaticAsset()
{}

ScriptAsset::~ScriptAsset()
{}

bool ScriptAsset::loadImpl(const std::string& filePath)
{
  return ScriptEngine::Get().isScriptRegistered(filePath);
}
