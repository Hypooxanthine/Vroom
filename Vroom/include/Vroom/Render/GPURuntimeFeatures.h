#pragma once

#include <cstdint>

namespace vrm
{

  struct GPURuntimeFeatures
  {
    
    void query();
    

    uint8_t maxMSAA = 0;
    uint32_t maxArrayTextureLayers = 0;
  };

} // namespace vrm
