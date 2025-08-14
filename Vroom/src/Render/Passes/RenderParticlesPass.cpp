#include "Vroom/Render/Passes/RenderParticlesPass.h"
#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "glm/fwd.hpp"

using namespace vrm;

namespace
{

  struct RawParticleStates
  {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool alive;
    float ellapsedLifeTime;
    float maxLifeTime;
  };

}

RenderParticlesPass::RenderParticlesPass()
{

}

RenderParticlesPass::~RenderParticlesPass()
{

}

void RenderParticlesPass::onInit()
{
  VRM_ASSERT_MSG(emitters != nullptr, "Invalid ptr reference to ParticleEmitterRegistry");

  MaterialAsset::Handle updaterAsset = AssetManager::Get().tryGetAsset<MaterialAsset>("Resources/Engine/Material/UpdateParticlesMaterial.json");
  VRM_CHECK_RET(updaterAsset.isValid());

  m_updaterMaterial = &getPassMaterial(updaterAsset);
}

void RenderParticlesPass::onSetup(const RenderPassContext &ctx)
{
  if (!m_updaterMaterial)
  {
    return;
  }

  if (emitters->wasJustModified())
  {
    _updateEmittersData();
    _updateParticleStates();
  }
}

void RenderParticlesPass::onRender(const RenderPassContext &ctx) const
{
  if (!m_updaterMaterial)
  {
    return;
  }
}

void RenderParticlesPass::_updateEmittersData()
{
  m_emittersDataBuffer.ensureCapacity(sizeof(ParticleEmitterSpecs) * emitters->getElementCount());
}

void RenderParticlesPass::_updateParticleStates()
{
  glm::uint maxParticleCount = 0;

  for (const ParticleEmitter* emitter : *emitters)
  {
    const auto& specs = emitter->getSpecs();
    maxParticleCount += static_cast<glm::uint>(specs.lifeTime * specs.emitRate * 1.5f) + 1u;
  }

  m_particleStatesBuffer.ensureCapacity(sizeof(RawParticleStates) * maxParticleCount, true);

  if (maxParticleCount > m_maxParticleCount)
  {
    // Setup particle states

    std::span<RawParticleStates> mapped = m_particleStatesBuffer.mapWriteOnly<RawParticleStates>(
      m_maxParticleCount * sizeof(RawParticleStates),
      (maxParticleCount - m_maxParticleCount) * sizeof(RawParticleStates),
      false
    );

    for (RawParticleStates& states : mapped)
    {
      states.alive = false; // So it can be spawned
      // The rest of the data is irrelevant since the particle is not alive
    }

    m_particleStatesBuffer.unmap();
  }

  m_maxParticleCount = maxParticleCount;
}
