#pragma once

#include <nlohmann/json.hpp>

#include "AssetManager/MaterialData.h"

namespace vrm
{

  class MaterialParsing
  {
  public:
    MaterialParsing() = delete;

    static bool Parse(const nlohmann::json &jsonData, MaterialData &outMaterialData);

  };

} // namespace vrm
