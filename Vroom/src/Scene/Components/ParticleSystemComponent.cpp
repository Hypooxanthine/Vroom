#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"

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

void ParticleSystemComponent::addEmitter(ParticleEmitter&& emitter)
{
  m_emitters.emplace_back(std::move(emitter));
}


