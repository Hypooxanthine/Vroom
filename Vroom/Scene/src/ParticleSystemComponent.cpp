#include "Scene/ParticleSystemComponent.h"
#include <cstddef>

#include "AssetManager/ParticleSystemAsset.h"
#include "Core/Log.h"
#include "Renderer/ParticleEmitterRender.h"

using namespace vrm;

ParticleSystemComponent::ParticleSystemComponent() {}

ParticleSystemComponent::ParticleSystemComponent(
  ParticleSystemAsset::Handle asset)
{
  setParticleSystem(asset);
}

ParticleSystemComponent::~ParticleSystemComponent() {}

void ParticleSystemComponent::setParticleSystem(
  ParticleSystemAsset::Handle asset)
{
  m_asset = asset;
  m_dirtyForRender = true;
}

void ParticleSystemComponent::addEmitter(ParticleEmitter::Specs&& specs)
{
  ParticleEmitter& emitter = m_emitters.emplace_back();
  emitter.setSpecs(std::move(specs));
  m_dirtyForRender = true;
}

void ParticleSystemComponent::removeEmitter(size_t id)
{
  if (id < m_emitters.size())
  {
    m_emitters.erase(m_emitters.begin() + id);
    m_dirtyForRender = true;
  }
  else VRM_LOG_ERROR("Could not remove emitter:: id {} is out of range", id);
}

void ParticleSystemComponent::update(const DeltaTime& dt)
{
  for (auto& emitter : m_emitters)
  {
    emitter.update(dt);
  }
}
