#pragma once

#include "AssetManager/Json.h"

#include "AssetManager/SceneData.h"

namespace nlohmann
{

  void VRM_ASSETMANAGER_API to_json(json& j, const vrm::SceneData& scene);

  void VRM_ASSETMANAGER_API from_json(const json& j, vrm::SceneData& scene);

}
