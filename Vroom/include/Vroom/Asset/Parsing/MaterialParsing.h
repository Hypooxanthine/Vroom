#pragma once

#include <nlohmann/json.hpp>

#include "Vroom/Asset/AssetData/MaterialData.h"

namespace vrm
{

  class MaterialParsing
  {
  public:
    MaterialParsing() = delete;

    static bool Parse(const nlohmann::json &jsonData, MaterialData &outSceneData);

  };

} // namespace vrm
