#pragma once

#include <cstdint>

namespace vrm
{

  struct RenderSettings
  {
    uint8_t antiAliasingLevel = 1;

    inline constexpr bool operator==(const RenderSettings& s) const
    {
      return true
        && antiAliasingLevel == s.antiAliasingLevel
      ;
    }
  };

} // namespace vrm
