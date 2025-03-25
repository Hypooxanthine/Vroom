#pragma once

#include <nlohmann/json.hpp>

#include "Vroom/Asset/AssetData/SceneData.h"

namespace vrm
{
  
class SceneParsing
{
public:
  SceneParsing() = delete;

  static bool Parse(const nlohmann::json& jsonData, SceneData& outSceneData);

  static void Serialize(const SceneData& sceneData, nlohmann::json& outJson);
};

} // namespace vrm
