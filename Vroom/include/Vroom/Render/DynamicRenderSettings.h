#pragma once

#include <cstdint>

namespace vrm
{
  
  struct DynamicRenderSettings
  {
    struct Shadows
    {
      uint8_t softShadowKernelRadius = 2;
    } shadows;

    struct Hdr
    {
      float exposure = 1.f;
    } hdr;

    inline constexpr bool operator==(const DynamicRenderSettings& s) const
    {
      return true
        && shadows.softShadowKernelRadius == s.shadows.softShadowKernelRadius
        && hdr.exposure == s.hdr.exposure
      ;
    }
  };

} // namespace vrm
