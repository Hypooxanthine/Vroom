#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/CubemapData.h"
#include "AssetManager/StaticAsset.h"
#include "RenderObjects/Cubemap.h"

namespace vrm
{

class VRM_ASSETMANAGER_API CubemapAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<CubemapAsset>;

  struct EFace
  {
    enum Type : size_t
    {
      ePosX = 0,
      eNegX,
      ePosY,
      eNegY,
      ePosZ,
      eNegZ,
      eCount
    };
  };

public:

  CubemapAsset();
  ~CubemapAsset();

  inline const CubemapData& getData() const
  {
    return m_data;
  }
  inline const render::Cubemap& getRenderCubemap() const
  {
    return m_renderCubemap;
  }

protected:

  bool loadImpl(const std::string& filePath) override;

private:

  bool _loadTexture(const std::string& filePath, ByteTextureData& textureData);
  bool _compareTextureWithNegX(const ByteTextureData& tex) const;
  bool _buildGpuCubemap();

private:

  CubemapData     m_data;
  render::Cubemap m_renderCubemap;
};

} // namespace vrm
