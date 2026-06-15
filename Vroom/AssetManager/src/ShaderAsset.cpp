#include "AssetManager/ShaderAsset.h"

#include "Core/Assert.h"
#include "AssetManager/JsonFile.h"
#include "AssetManager/ShaderParsing.h"

#include <nlohmann/json.hpp>

using namespace vrm;

bool ShaderAsset::loadImpl(const std::filesystem::path& filePath)
{
  VRM_LOG_TRACE("Loading shader: {}", filePath.string());

  nlohmann::json j;
  if (!ReadJsonFile(filePath, j))
    return false;

  return ShaderParsing::Parse(j, m_data);
}