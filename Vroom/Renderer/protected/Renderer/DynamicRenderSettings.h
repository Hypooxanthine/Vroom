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

      inline constexpr bool operator==(const Hdr& s) const
      {
        return true
          && exposure == s.exposure
        ;
      }
    } hdr;

    struct Bloom
    {
      float intensity = 1.f;
      float threshold = 1.f;
      uint8_t blurPasses = 5;

      inline constexpr bool operator==(const Bloom& s) const
      {
        return true
          && intensity == s.intensity
          && threshold == s.threshold
          && blurPasses == s.blurPasses
        ;
      }
    } bloom;

    inline constexpr bool operator==(const DynamicRenderSettings& s) const
    {
      return true
        && shadows.softShadowKernelRadius == s.shadows.softShadowKernelRadius
        && hdr == s.hdr
        && bloom == s.bloom
      ;
    }
  };

} // namespace vrm
