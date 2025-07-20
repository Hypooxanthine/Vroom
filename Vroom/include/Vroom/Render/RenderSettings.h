#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace vrm
{

  struct RenderSettings
  {
    uint16_t frameRateLimit = 0; // 0 for unlimited
    uint8_t antiAliasingLevel = 1;
    bool shadowsEnable = true;

    struct NormalMappingSettings
    {
      bool activated = true;
      bool reorthoTangentSpace = false;

      inline constexpr bool operator==(const NormalMappingSettings& s) const
      {
        return true
          && activated == s.activated
          && reorthoTangentSpace == s.reorthoTangentSpace
        ;
      }
    } normalMapping;

    struct BloomSettings
    {
      bool activated = true;

      inline constexpr bool operator==(const BloomSettings& s) const
      {
        return true
          && activated == s.activated
        ;
      }
    } bloom;

    bool clusteredShading = true;
    glm::uvec3 clusterCount = { 12, 12, 24 };

    // Render debug
    bool showLightComplexity = false;

    inline constexpr bool operator==(const RenderSettings& s) const
    {
      return true
        && frameRateLimit == s.frameRateLimit
        && antiAliasingLevel == s.antiAliasingLevel
        && shadowsEnable == s.shadowsEnable
        && normalMapping == s.normalMapping
        && bloom == s.bloom
        && clusteredShading == s.clusteredShading
        && clusterCount == s.clusterCount
        && showLightComplexity == s.showLightComplexity
      ;
    }
  };

} // namespace vrm
