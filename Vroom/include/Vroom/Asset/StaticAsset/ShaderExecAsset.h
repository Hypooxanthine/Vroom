#pragma once

#include <nlohmann/json.hpp>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include "Vroom/Asset/AssetInstance/ShaderExecInstance.h"

namespace vrm
{

class ShaderExecAsset : public StaticAsset
{
  VRM_DECLARE_STATIC_ASSET(ShaderExec)

public:
  ShaderExecAsset() = default;
  ~ShaderExecAsset() = default;
  
  const auto& getShaderExecData() const { return m_ShaderExecData; }

private:
  nlohmann::json m_ShaderExecData;
};

} // namespace vrm
