#pragma once

#include <span>
#include <vector>

#include "AssetManager/ParticleSystemAsset.h"
#include "Renderer/ParticleEmitter.h"
#include "Scene/Api.h"

namespace vrm
{

class DeltaTime;

class VRM_SCENE_API ParticleSystemComponent
{
public:

  ParticleSystemComponent();
  ParticleSystemComponent(const ParticleSystemAsset::Handle& asset);
  ~ParticleSystemComponent();

  void setParticleSystem(const ParticleSystemAsset::Handle& asset);

  void addEmitter(ParticleEmitter::Specs&& specs);
  void removeEmitter(size_t id);

  bool consumeDirtyForRender() const
  {
    bool value = m_dirtyForRender;
    m_dirtyForRender = false;

    return value;
  }

  void markDirtyForRender() const
  {
    m_dirtyForRender = true;
  }

  void update(const DeltaTime& dt);

  std::span<ParticleEmitter const> getEmitters() const
  {
    return std::span{ m_emitters };
  }
  std::span<ParticleEmitter> getEmitters()
  {
    return std::span{ m_emitters };
  }

  ParticleSystemComponent(const ParticleSystemComponent&) = delete;
  ParticleSystemComponent& operator=(const ParticleSystemComponent&) = delete;

  ParticleSystemComponent(ParticleSystemComponent&&) = delete;
  ParticleSystemComponent& operator=(ParticleSystemComponent&&) = delete;

private:

  ParticleSystemAsset::Handle m_asset = {};

  std::vector<ParticleEmitter> m_emitters;
  mutable bool m_dirtyForRender = true;
};

} // namespace vrm
