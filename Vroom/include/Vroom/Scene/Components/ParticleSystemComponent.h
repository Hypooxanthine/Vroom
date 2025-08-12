#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"

namespace vrm
{
  
  class VRM_API ParticleSystemComponent
  {
  public:

    ParticleSystemComponent();
    ParticleSystemComponent(ParticleSystemAsset::Handle asset);
    ~ParticleSystemComponent();

    void setParticleSystem(ParticleSystemAsset::Handle asset);

    ParticleSystemComponent(const ParticleSystemComponent&) = delete;
    ParticleSystemComponent& operator=(const ParticleSystemComponent&) = delete;

    ParticleSystemComponent(ParticleSystemComponent&&) = delete;
    ParticleSystemComponent& operator=(ParticleSystemComponent&&) = delete;

  private:

    ParticleSystemAsset::Handle m_asset = {};

  };
  
} // namespace vrm
