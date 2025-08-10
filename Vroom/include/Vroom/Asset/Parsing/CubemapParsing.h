#pragma once

#include <nlohmann/json.hpp>
#include "Vroom/Api.h"
#include "Vroom/Asset/AssetData/CubemapData.h"

namespace nlohmann
{
  void VRM_API to_json(json& j, const vrm::CubemapData::JsonLayout& cubemap);

  void VRM_API from_json(const json& j, vrm::CubemapData::JsonLayout& cubemap);
  
} // namespace nlohmann
