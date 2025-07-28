#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/CubemapAsset.h"

namespace vrm
{
  
  class VRM_API SkyboxComponent
  {
  public:

    SkyboxComponent();
    SkyboxComponent(CubemapAsset::Handle cubemap);
    ~SkyboxComponent();

    inline CubemapAsset::Handle getCubemapAsset() const { return m_cubemap; }
    inline void setCubemapAsset(CubemapAsset::Handle cubemap) { m_cubemap = cubemap; }

  private:

    CubemapAsset::Handle m_cubemap;    
  };

} // namespace vrm
