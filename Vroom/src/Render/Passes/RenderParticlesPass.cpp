#include "Vroom/Render/Passes/RenderParticlesPass.h"
#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"
#include "Vroom/Render/RenderView.h"
#include "glm/fwd.hpp"
#include <cstring>
#include <vector>

using namespace vrm;

namespace
{

  struct RawEmitterData
  {
    glm::uint particlesToSpawn;
    float firstParticleStamp;
    ParticleEmitterSpecs specs;
  };

  struct RawParticleStates
  {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec4 color;
    glm::uint alive;
    float ellapsedLifeTime;
    float maxLifeTime;
  };

  struct RawInstanceData
  {
    glm::mat4 modelMatrix;
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
  if (!m_updaterMaterial || emitters->getElementCount() == 0)
  {
    return;
  }

  if (emitters->wasJustModified())
  {
    _updateEmittersData();
    _updateParticleStates();
  }

  // Every frame to reset instance counts to zero
  _uploadIndirectCommandsData();
}

void RenderParticlesPass::onRender(const RenderPassContext &ctx) const
{
  if (!m_updaterMaterial || emitters->getElementCount() == 0)
  {
    return;
  }

  const gl::Shader& updaterShader = m_updaterMaterial->getShader();
  updaterShader.bind();
  _bindEmittersData(updaterShader);
  _bindIndirectCommands(updaterShader);
  _bindParticleStates(updaterShader);
  _bindParticleInstanceData(updaterShader);
  updaterShader.setUniform1ui("u_maxParticleCount", m_maxParticleCount);
  updaterShader.setUniform1f("u_deltaTime", Application::Get().getDeltaTime().seconds());

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  glm::uvec3 dispatch = gl::Shader::ComputeDispatchSize(s_updaterGroupSize, glm::uvec3(m_maxParticleCount, 1, 1));
  GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  gl::Buffer::Bind(m_indirectCommandsBuffer.getBuffer(), GL_DRAW_INDIRECT_BUFFER);

  for (size_t i = 0; i < emitters->getElementCount(); ++i)
  {
    const ParticleEmitter& emitter = *emitters->at(i);
    const RenderMesh& mesh = emitter.getMesh()->getSubMeshes().at(0).renderMesh;
    const gl::Shader& shader = m_particleMaterials.at(i)->getShader();

    gl::VertexArray::Bind(mesh.getVertexArray());
    gl::Buffer::Bind(mesh.getIndexBuffer(), GL_ELEMENT_ARRAY_BUFFER);
    shader.bind();
    _bindParticleInstanceData(shader);

    for (const render::View& view : ctx.views)
    {
      const auto& vpOrigin = view.getViewport().getOrigin();
      const auto& vpSize = view.getViewport().getSize();
      glViewport(vpOrigin.x, vpOrigin.y, vpSize.x, vpSize.y);
      shader.setUniformMat4f("u_viewProj", view.getCamera()->getViewProjection());

      GLCall(
        glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr)
      );
    }
  }
}

void RenderParticlesPass::_updateEmittersData()
{
  m_emittersDataBuffer.ensureCapacity(sizeof(RawEmitterData) * emitters->getElementCount());
  m_indirectCommandsBuffer.ensureCapacity(sizeof(RawDrawElementsIndirectCommand) * emitters->getElementCount());

  m_indirectCommands.resize(emitters->getElementCount());
  m_particleMaterials.resize(emitters->getElementCount(), nullptr);

  for (size_t i = 0; i < emitters->getElementCount(); ++i)
  {
    const ParticleEmitter& emitter = *emitters->at(i);
    RawDrawElementsIndirectCommand& cmd = m_indirectCommands.at(i);
    const RenderMesh& mesh = emitter.getMesh()->getSubMeshes().at(0).renderMesh;

    cmd.count = mesh.getIndexCount();
    cmd.firstIndex = 0;
    cmd.baseVertex = 0;
    cmd.baseInstance = 0;
    cmd.instanceCount = 0;

    m_particleMaterials.at(i) = &getPassMaterial(emitter.getMaterial());
  }
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
  m_instanceDataBuffer.ensureCapacity(sizeof(RawInstanceData) * maxParticleCount, false);

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
      states.alive = 0; // So it can be spawned
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

void RenderParticlesPass::_bindSpawnCounters(const gl::Shader& shader) const
{
  shader.setStorageBuffer("SpawnCountersBlock", m_emittersDataBuffer.getBuffer());
}

void RenderParticlesPass::_bindIndirectCommands(const gl::Shader& shader) const
{
  shader.setStorageBuffer("IndirectCommandsBlock", m_indirectCommandsBuffer.getBuffer());
}

void RenderParticlesPass::_bindParticleStates(const gl::Shader& shader) const
{
  shader.setStorageBuffer("ParticleStatesBlock", m_particleStatesBuffer.getBuffer());
}

void RenderParticlesPass::_bindParticleInstanceData(const gl::Shader& shader) const
{
  shader.setStorageBuffer("ParticleInstanceDataBlock", m_instanceDataBuffer.getBuffer());
}

void RenderParticlesPass::_uploadIndirectCommandsData() const
{
  if (m_indirectCommands.size() > 0)
  {
    std::span<RawDrawElementsIndirectCommand> mapped = m_indirectCommandsBuffer.mapWriteOnly<RawDrawElementsIndirectCommand>(
      0,
      m_indirectCommands.size() * sizeof(RawDrawElementsIndirectCommand),
      true // No cpu stall if resource is used
    );

    std::memcpy(mapped.data(), m_indirectCommands.data(), m_indirectCommands.size() * sizeof(RawDrawElementsIndirectCommand));

    m_indirectCommandsBuffer.unmap();
  }
}
