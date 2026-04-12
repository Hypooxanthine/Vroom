#pragma once

#include "AssetManager/Api.h"

#include "AssetManager/ComponentData.h"

namespace nlohmann
{

  void VRM_ASSETMANAGER_API to_json(json& j, const vrm::ComponentData& component);

  void VRM_ASSETMANAGER_API from_json(const json& j, std::unique_ptr<vrm::ComponentData>& component);

}
