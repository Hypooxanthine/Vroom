#include "Vroom/Render/Passes/RenderParticlesPass.h"
#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Shader.h"
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

  addDefine("VRM_UPDATER_GROUP_SIZE", s_updaterGroupSize.x);

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

  const gl::Shader& updaterShader = m_updaterMaterial->getShader();
  updaterShader.bind();
  _bindEmittersData(updaterShader);
  _bindParticleStates(updaterShader);
  updaterShader.setUniform1ui("u_maxParticleCount", m_maxParticleCount);
  updaterShader.setUniform1f("u_deltaTime", Application::Get().getDeltaTime().seconds());

  glm::uvec3 dispatch = gl::Shader::ComputeDispatchSize(s_updaterGroupSize, glm::uvec3(m_maxParticleCount, 1, 1));
  GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));
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
    // Setup new particle states

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

void RenderParticlesPass::_bindEmittersData(const gl::Shader& shader) const
{
  shader.setStorageBuffer("EmittersDataBlock", m_emittersDataBuffer.getBuffer());
}

void RenderParticlesPass::_bindParticleStates(const gl::Shader& shader) const
{
  shader.setStorageBuffer("ParticleStatesBlock", m_particleStatesBuffer.getBuffer());
}
