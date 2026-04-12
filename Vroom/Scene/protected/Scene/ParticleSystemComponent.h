#pragma once

#include <span>
#include <vector>

#include "Scene/Api.h"
#include "AssetManager/ParticleSystemAsset.h"
#include "Renderer/ParticleEmitter.h"

namespace vrm
{

class DeltaTime;

class VRM_SCENE_API ParticleSystemComponent
{
public:

  ParticleSystemComponent();
  ParticleSystemComponent(ParticleSystemAsset::Handle asset);
  ~ParticleSystemComponent();

  void setParticleSystem(ParticleSystemAsset::Handle asset);

  void addEmitter(ParticleEmitter::Specs&& specs);
  void removeEmitter(size_t id);

  void update(const DeltaTime& dt);

  std::span<ParticleEmitter const> getEmitters() const
  {
    return std::span{ m_emitters };
  }
  std::span<ParticleEmitter> getEmitters() { return std::span{ m_emitters }; }

  ParticleSystemComponent(const ParticleSystemComponent&)            = delete;
  ParticleSystemComponent& operator=(const ParticleSystemComponent&) = delete;

  ParticleSystemComponent(ParticleSystemComponent&&)            = delete;
  ParticleSystemComponent& operator=(ParticleSystemComponent&&) = delete;

private:

  ParticleSystemAsset::Handle m_asset = {};

  std::vector<ParticleEmitter> m_emitters;
};

} // namespace vrm
