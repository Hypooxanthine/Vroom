#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"
#include "Vroom/Core/Log.h"
#include "Vroom/Render/ParticleEmitterRender.h"
#include <cstddef>

using namespace vrm;

ParticleSystemComponent::ParticleSystemComponent()
{

}

ParticleSystemComponent::ParticleSystemComponent(ParticleSystemAsset::Handle asset)
{
  setParticleSystem(asset);
}

ParticleSystemComponent::~ParticleSystemComponent()
{
  
}

void ParticleSystemComponent::setParticleSystem(ParticleSystemAsset::Handle asset)
{
  m_asset = asset;
}

void ParticleSystemComponent::addEmitter(const ParticleEmitter::Specs& specs)
{
  ParticleEmitter& emitter = m_emitters.emplace_back();
  emitter.setSpecs(specs);
}

void ParticleSystemComponent::removeEmitter(size_t id)
{
  if (id < m_emitters.size())
    m_emitters.erase(m_emitters.begin() + id);
  else
    VRM_LOG_ERROR("Could not remove emitter:: id {} is out of range", id);
}

void ParticleSystemComponent::update(const DeltaTime& dt)
{
  for (auto& emitter : m_emitters)
  {
    emitter.update(dt);
  }
}


