#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetData/CubemapData.h"

namespace vrm
{
  
  class VRM_API CubemapAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<CubemapAsset>;

    struct EFace
    {
      enum Type : size_t
      {
        ePosX = 0, eNegX, ePosY, eNegY, ePosZ, eNegZ,
        eCount
      };
    };

  public:

    CubemapAsset();
    ~CubemapAsset();

    inline const CubemapData& getData() const { return m_data; }

  protected:
    bool loadImpl(const std::string& filePath) override;

  private:

    bool _loadTexture(const std::string& filePath, ByteTextureData& textureData);
    bool _compareTextureWithNegX(const ByteTextureData& tex) const;

  private:

    CubemapData m_data;
  };

} // namespace vrm
