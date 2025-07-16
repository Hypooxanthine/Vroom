#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"

namespace vrm
{
  
  struct VRM_API CubemapData
  {
    struct JsonLayout
    {
      std::string posx, negx, posy, negy, posz, negz;
    };

    // Big problem here, TextureAsset is creating OpenGL textures, but we don't need those 6 textures,
    // we only need 1 texture cube
    // This is a huge problem rn, openGL resources should not be created by assets AT ALL.
    TextureAsset::Handle posx, negx, posy, negy, posz, negz;
  };

} // namespace vrm
