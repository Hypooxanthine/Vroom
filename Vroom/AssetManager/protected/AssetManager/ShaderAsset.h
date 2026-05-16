#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/AssetHandle.h"
#include "AssetManager/ShaderData.h"
#include "AssetManager/StaticAsset.h"


namespace vrm
{

class VRM_ASSETMANAGER_API ShaderAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<ShaderAsset>;

public:

  ShaderAsset()  = default;
  ~ShaderAsset() = default;

  bool loadImpl(const std::filesystem::path& filePath) override;

  const auto& getShaderData() const
  {
    return m_data;
  }

private:

  ShaderData m_data;
};

} // namespace vrm
