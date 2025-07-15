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
    uint8_t softShadowKernelRadius = 2;

    struct NormalMappingSettings
    {
      bool activated = true;
      bool reorthoTangentSpace = false;
    } normalMapping;

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
        && softShadowKernelRadius == s.softShadowKernelRadius
        && normalMapping.activated == s.normalMapping.activated
        && normalMapping.reorthoTangentSpace == s.normalMapping.reorthoTangentSpace
        && clusteredShading == s.clusteredShading
        && clusterCount == s.clusterCount
        && showLightComplexity == s.showLightComplexity
      ;
    }
  };

} // namespace vrm
