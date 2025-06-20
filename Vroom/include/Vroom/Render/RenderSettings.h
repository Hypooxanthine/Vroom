#pragma once

#include <cstdint>

namespace vrm
{

  struct RenderSettings
  {
    uint16_t frameRateLimit = 0; // 0 for unlimited
    uint8_t antiAliasingLevel = 1;
    bool shadowsEnable = true;
    uint8_t softShadowKernelRadius = 2;

    inline constexpr bool operator==(const RenderSettings& s) const
    {
      return true
        && frameRateLimit == s.frameRateLimit
        && antiAliasingLevel == s.antiAliasingLevel
        && shadowsEnable == s.shadowsEnable
        && softShadowKernelRadius == s.softShadowKernelRadius
      ;
    }
  };

} // namespace vrm
