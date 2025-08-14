#pragma once

#include "Vroom/DataStructure/LinearRegistry.h"
#include "Vroom/Render/ParticleEmitter.h"
#include <cstddef>

namespace vrm
{

  using ParticleEmitterRegistry = LinearRegistry<const ParticleEmitter*, size_t>;

}