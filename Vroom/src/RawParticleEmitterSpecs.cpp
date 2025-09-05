#include "Vroom/Render/RawParticleEmitterSpecs.h"

using namespace vrm;

RawParticleEmitterSpecs::RawParticleEmitterSpecs() {}

RawParticleEmitterSpecs::~RawParticleEmitterSpecs() {}

void RawParticleEmitterSpecs::setupLayout() {}

void RawParticleEmitterSpecs::_clear()
{
  delete m_data;
  m_data     = nullptr;
  m_dataSize = 0;
}
