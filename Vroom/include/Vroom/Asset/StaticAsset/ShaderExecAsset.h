#pragma once

#include <nlohmann/json.hpp>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include "Vroom/Asset/AssetInstance/ShaderExecInstance.h"

namespace vrm
{

class ShaderExecAsset : public StaticAsset
{
  VRM_DECLARE_STATIC_ASSET(ShaderExec)

private:
  nlohmann::json m_ShaderExecData;
};

} // namespace vrm
