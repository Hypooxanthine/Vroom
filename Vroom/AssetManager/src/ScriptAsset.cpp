#include "AssetManager/ScriptAsset.h"

#include "ScriptEngine/ScriptEngine.h"

using namespace vrm;

ScriptAsset::ScriptAsset() : StaticAsset()
{}

ScriptAsset::~ScriptAsset()
{}

bool ScriptAsset::loadImpl(const std::filesystem::path& filePath)
{
  return ScriptEngine::Get().isScriptRegistered(filePath.string());
}
