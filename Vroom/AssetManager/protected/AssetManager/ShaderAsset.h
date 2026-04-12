#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/StaticAsset.h"

#include "AssetManager/ShaderData.h"

namespace vrm
{

  class VRM_ASSETMANAGER_API ShaderAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<ShaderAsset>;
  public:
    ShaderAsset() = default;
    ~ShaderAsset() = default;

    bool loadImpl(const std::string& filePath) override;

    const auto& getShaderData() const { return m_data; }

  private:
    ShaderData m_data;
  };

} // namespace vrm