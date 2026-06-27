#pragma once

#include <cstdint>
#include "Physics/PhysicsLayerList.h"

namespace vrm
{

struct PhysicsSettings
{
  uint32_t maxBodies = 65536;
  uint32_t maxBodyPairs = 65536;
  uint32_t maxContactConstraints = 10240;
  PhysicsLayerList layers;
};

}; // namespace vrm
