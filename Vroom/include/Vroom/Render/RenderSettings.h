#pragma once

#include <cstdint>

namespace vrm
{

  struct RenderSettings
  {
    uint8_t antiAliasingLevel = 1;
    bool shadowsEnable = true;

    inline constexpr bool operator==(const RenderSettings& s) const
    {
      return true
        && antiAliasingLevel == s.antiAliasingLevel
        && shadowsEnable == s.shadowsEnable
      ;
    }
  };

} // namespace vrm
