#pragma once

#include "Vroom/Asset/StaticAsset/CubemapAsset.h"

namespace vrm
{
  
  class RenderSkybox
  {
  public:

    RenderSkybox() = default;
    ~RenderSkybox() = default;

    inline void clear() { m_cubemap = {}; }

    inline void setCubemap(CubemapAsset::Handle cm) { m_cubemap = cm; }
    inline CubemapAsset::Handle getCubemap() const { return m_cubemap; }

  private:

    CubemapAsset::Handle m_cubemap;
  };

} // namespace vrm
