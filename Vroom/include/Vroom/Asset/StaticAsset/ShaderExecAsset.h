#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ShaderExecInstance.h"
#include "Vroom/Asset/AssetData/ShaderData.h"

namespace vrm
{

class ShaderExecAsset : public StaticAsset
{
  VRM_DECLARE_STATIC_ASSET(ShaderExec)

public:
  ShaderExecAsset() = default;
  ~ShaderExecAsset() = default;
  
  const auto& getShaderData() const { return m_data; }

private:
  ShaderData m_data;
};

} // namespace vrm
