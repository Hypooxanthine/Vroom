#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/AssetHandle.h"
#include "AssetManager/ShaderData.h"
#include "AssetManager/StaticAsset.h"


namespace vrm
{

class VRM_ASSETMANAGER_API ShadingModelAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<ShadingModelAsset>;

public:

  ShadingModelAsset();
  ~ShadingModelAsset();

  [[nodiscard]] inline const ShaderData& getData() const
  {
    return m_data;
  }

protected:

  bool loadImpl(const std::filesystem::path& filePath) override;

private:

  ShaderData m_data;
};

} // namespace vrm
