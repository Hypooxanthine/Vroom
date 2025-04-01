#include "Vroom/Asset/StaticAsset/SceneAsset.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/AssetData/SceneData.h"
#include "Vroom/Asset/Parsing/SceneParsing.h"

using namespace vrm;

SceneAsset::SceneAsset()
{
}

SceneAsset::~SceneAsset()
{
}

bool SceneAsset::loadImpl(const std::string& filePath)
{
  using json = nlohmann::json;

  std::ifstream ifs;
  ifs.open(filePath);
  VRM_CHECK_RET_FALSE_MSG(ifs.is_open(), "Could not open {}", filePath);

  json jsonData;

  try
  {
    ifs >> jsonData;
  }
  catch (const std::exception& e)
  {
    VRM_LOG_ERROR("Malformed json:\n{}", e.what());
    return false;
  }

  m_data.reset(new SceneData());

  VRM_CHECK_RET_FALSE_MSG(SceneParsing::Parse(jsonData, *m_data), "Error while parsing scene from json");

  return true;
}
