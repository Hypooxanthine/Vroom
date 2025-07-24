#pragma once

#include "Vroom/Api.h"

namespace vrm
{
  
  class VRM_API ParticleSystemComponent
  {
  public:

    ParticleSystemComponent();
    ~ParticleSystemComponent();

    ParticleSystemComponent(const ParticleSystemComponent&) = delete;
    ParticleSystemComponent& operator=(const ParticleSystemComponent&) = delete;

    ParticleSystemComponent(ParticleSystemComponent&&) = delete;
    ParticleSystemComponent& operator=(ParticleSystemComponent&&) = delete;

  private:

  };
  
} // namespace vrm
