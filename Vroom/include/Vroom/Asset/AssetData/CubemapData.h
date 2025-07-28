#pragma once

#include "Vroom/Api.h"
#include "Vroom/Render/Abstraction/Texture.h"

namespace vrm
{
  
  struct VRM_API CubemapData
  {
    struct JsonLayout
    {
      std::string posx, negx, posy, negy, posz, negz;
    };

    ByteTextureData posx, negx, posy, negy, posz, negz;
  };

} // namespace vrm
