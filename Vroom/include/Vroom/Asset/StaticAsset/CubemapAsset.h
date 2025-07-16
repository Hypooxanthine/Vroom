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

  public:

    CubemapAsset();
    ~CubemapAsset();

    inline const CubemapData& getData() const { return m_data; }

  protected:
    bool loadImpl(const std::string& filePath) override;

  private:

    CubemapData m_data;
  };

} // namespace vrm
