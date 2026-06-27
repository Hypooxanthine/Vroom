#pragma once

#include <cstdint>

namespace vrm
{

enum class EPhysicsLayerGroup : uint8_t
{
  eStatic = 0,
  eDynamic,
  Count
};

}
