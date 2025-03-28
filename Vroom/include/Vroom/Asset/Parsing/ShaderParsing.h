#pragma once

#include <nlohmann/json.hpp>

#include <string>

#include "Vroom/Asset/AssetData/ShaderData.h"

namespace vrm
{

class ShaderParsing
{
public:
  ShaderParsing() = delete;

  static bool Parse(const nlohmann::json& jsonData, ShaderData& out);

private:

};

} // namespace vrm
