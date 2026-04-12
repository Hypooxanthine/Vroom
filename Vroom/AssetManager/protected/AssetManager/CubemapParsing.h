#pragma once

#include <nlohmann/json.hpp>
#include "AssetManager/Api.h"
#include "AssetManager/CubemapData.h"

namespace nlohmann
{
  void VRM_ASSETMANAGER_API to_json(json& j, const vrm::CubemapData::JsonLayout& cubemap);

  void VRM_ASSETMANAGER_API from_json(const json& j, vrm::CubemapData::JsonLayout& cubemap);
  
} // namespace nlohmann
