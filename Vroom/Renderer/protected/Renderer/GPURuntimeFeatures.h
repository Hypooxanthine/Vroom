#pragma once

#include <cstdint>
#include "Renderer/Api.h"

namespace vrm
{

  struct VRM_RENDERER_API GPURuntimeFeatures
  {
    
    inline bool wasQueried() const { return m_queried; }
    void query();

    uint8_t maxMSAA = 0;
    uint32_t maxArrayTextureLayers = 0;

  private:
    bool m_queried = false;
  };

} // namespace vrm
