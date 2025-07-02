#pragma once

#include "Vroom/Asset/Parsing/Json.h"

#include "Vroom/Asset/AssetData/SceneData.h"

namespace nlohmann
{

  void VRM_API to_json(json& j, const vrm::SceneData& scene);

  void VRM_API from_json(const json& j, vrm::SceneData& scene);

}
