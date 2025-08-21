#pragma once

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"
#include "Vroom/Render/ParticleEmitter.h"
#include <span>
#include <vector>

namespace vrm
{

  class DeltaTime;
  
  class VRM_API ParticleSystemComponent
  {
  public:

    ParticleSystemComponent();
    ParticleSystemComponent(ParticleSystemAsset::Handle asset);
    ~ParticleSystemComponent();

    void setParticleSystem(ParticleSystemAsset::Handle asset);

    void addEmitter(const ParticleEmitter::Specs& specs);

    void update(const DeltaTime& dt);

    std::span<ParticleEmitter const> getEmitters() const { return std::span{ m_emitters }; }
    std::span<ParticleEmitter> getEmitters() { return std::span{ m_emitters }; }

    ParticleSystemComponent(const ParticleSystemComponent&) = delete;
    ParticleSystemComponent& operator=(const ParticleSystemComponent&) = delete;

    ParticleSystemComponent(ParticleSystemComponent&&) = delete;
    ParticleSystemComponent& operator=(ParticleSystemComponent&&) = delete;

  private:

    ParticleSystemAsset::Handle m_asset = {};

    std::vector<ParticleEmitter> m_emitters;

  };
  
} // namespace vrm
