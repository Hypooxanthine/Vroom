#include "AssetManager/ScriptAsset.h"

#include "ScriptEngine/ScriptEngine.h"

using namespace vrm;

ScriptAsset::ScriptAsset() : StaticAsset()
{}

ScriptAsset::~ScriptAsset()
{}

bool ScriptAsset::loadImpl(const std::string& filePath)
{
  return ScriptEngine::Get().isScriptRegistered(filePath);
}
