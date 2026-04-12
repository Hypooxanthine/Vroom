#pragma once

#include "AssetManager/Api.h"
#include "AssetManager/TextureData.h"

namespace vrm
{
  
  struct VRM_ASSETMANAGER_API CubemapData
  {
    struct JsonLayout
    {
      std::string posx, negx, posy, negy, posz, negz;
    };

    ByteTextureData posx, negx, posy, negy, posz, negz;
  };

} // namespace vrm
