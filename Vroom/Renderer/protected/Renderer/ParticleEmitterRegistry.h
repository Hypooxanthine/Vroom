#pragma once

#include "Tools/LinearRegistry.h"
#include "Renderer/ParticleEmitter.h"
#include <cstddef>

namespace vrm
{

  struct ParticleSystemRenderInfo
  {
    const ParticleEmitter* emitter;
		const glm::mat4* model;
  };

  using ParticleEmitterRegistry = LinearRegistry<ParticleSystemRenderInfo, size_t>;

}