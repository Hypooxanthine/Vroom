#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ShaderInstance.h"

#include "Vroom/Asset/AssetData/ShaderData.h"

namespace vrm
{

  class ShaderAsset : public StaticAsset
  {
    VRM_DECLARE_STATIC_ASSET(Shader)

  public:
    ShaderAsset() = default;
    ~ShaderAsset() = default;

    const auto& getShaderData() const { return m_data; }

  private:
    ShaderData m_data;
  };

} // namespace vrm