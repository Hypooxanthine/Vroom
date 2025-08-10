#pragma once

#include "Vroom/Api.h"

#include "Vroom/Asset/AssetData/ComponentData.h"

namespace nlohmann
{

  void VRM_API to_json(json& j, const vrm::ComponentData& component);

  void VRM_API from_json(const json& j, std::unique_ptr<vrm::ComponentData>& component);

}
