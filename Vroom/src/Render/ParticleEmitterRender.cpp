#include "Vroom/Render/ParticleEmitterRender.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/RenderView.h"
#include "glm/glm.hpp"

using namespace vrm;

namespace
{

struct RawInstanceData
{
  glm::mat4 modelMatrix;
  glm::vec4 color;
};

struct RawEmitterSpawnData
{
  glm::uint atomicCounter = 0;
};

} // namespace

ParticleEmitterRender::ParticleEmitterRender()
{
  m_spawnDataBuffer.ensureCapacity(sizeof(RawEmitterSpawnData));
  m_indirectCommandsBuffer.ensureCapacity(
    sizeof(RawDrawElementsIndirectCommand));
}

ParticleEmitterRender::~ParticleEmitterRender() {}

void ParticleEmitterRender::setMesh(MeshAsset::Handle mesh)
{
  if (mesh == m_mesh) return;

  m_mesh = mesh;
  if (!m_mesh.isValid()) return;

  m_indirectCommand.baseInstance  = 0;
  m_indirectCommand.instanceCount = 0;
  m_indirectCommand.baseVertex    = 0;
  m_indirectCommand.firstIndex    = 0;
  m_indirectCommand.count =
    mesh->getSubMeshes().front().renderMesh.getIndexCount();
}

void ParticleEmitterRender::setRenderMaterial(MaterialAsset::Handle material)
{
  m_renderMaterial.setMaterialAsset(material);
  m_renderMaterial.prepare({});
}

namespace
{

template <class... Ts>
struct overloads : Ts...
{
  using Ts::operator()...;
};

template <class... Ts>
overloads(Ts...) -> overloads<Ts...>;

} // namespace

void ParticleEmitterRender::rebuildMaterials(const ParticleEmitter& emitter)
{
  m_rawEmitterSpecsLayout.reset();

  const ParticleEmitter::Specs& specs = emitter.getSpecs();
  MaterialDefines               defines;

  const auto applyAttrib = [&](IEmitterAttrib& attrib) -> void
  {
    static const std::string prefix = "VRM_PARTICLE";
    attrib.pushToLayout(m_rawEmitterSpecsLayout);
    attrib.applyDefines(defines, prefix);
  };

  applyAttrib(*specs.emitRate);
  applyAttrib(*specs.lifeTime);
  applyAttrib(*specs.spawnPosition);
  applyAttrib(*specs.spawnVelocity);
  applyAttrib(*specs.spawnScale);
  applyAttrib(*specs.spawnColor);

  m_rawEmitterSpecs.setup(m_rawEmitterSpecsLayout, 1);

  _setupUpdaterMaterial(defines);
}

void ParticleEmitterRender::updateResources(const ParticleEmitter& emitter)
{
  _updateParticleStates(emitter);
  _updateEmitterData(emitter);
}

void ParticleEmitterRender::prepareFrame(const ParticleEmitter& emitter)
{
  _uploadSpawnData(emitter);
  _uploadIndirectCommandData();
}

void ParticleEmitterRender::executeRender(const ParticleEmitter&   emitter,
                                          const RenderPassContext& ctx,
                                          const glm::mat4*         model)
{
  _executeUpdateParticles(emitter);
  _executeRenderParticles(ctx, model);
}

void ParticleEmitterRender::_setupUpdaterMaterial(
  const MaterialDefines& defines)
{
  m_emitterDataBuffer.ensureCapacity(m_rawEmitterSpecs.getSize());

  MaterialAsset::Handle updaterAsset =
    AssetManager::Get().tryGetAsset<MaterialAsset>(
      "Resources/Engine/Material/UpdateParticlesMaterial.json");
  VRM_CHECK_RET(updaterAsset.isValid());

  MaterialDefines localDefines;
  localDefines.add(defines);
  localDefines.add("VRM_UPDATER_GROUP_SIZE", s_updaterGroupSize.x);

  m_updaterMaterial.setMaterialAsset(updaterAsset);
  m_updaterMaterial.prepare(localDefines);
}

void ParticleEmitterRender::_uploadSpawnData(
  const ParticleEmitter& emitter) const
{
  std::span<RawEmitterSpawnData> mapped =
    m_spawnDataBuffer.mapWriteOnly<RawEmitterSpawnData>(
      0, sizeof(RawEmitterSpawnData), true);

  RawEmitterSpawnData& rawData = mapped[0];
  rawData.atomicCounter        = 0;

  m_spawnDataBuffer.unmap();
}

void ParticleEmitterRender::_uploadIndirectCommandData() const
{
  std::span<RawDrawElementsIndirectCommand> mapped =
    m_indirectCommandsBuffer.mapWriteOnly<RawDrawElementsIndirectCommand>(
      0, sizeof(RawDrawElementsIndirectCommand),
      true // No cpu stall if resource is used
    );

  mapped[0] = m_indirectCommand;

  m_indirectCommandsBuffer.unmap();
}

void ParticleEmitterRender::_updateParticleStates(
  const ParticleEmitter& emitter)
{
  const auto& specs = emitter.getSpecs();

  glm::uint maxParticleCount = static_cast<glm::uint>(
    specs.lifeTime->getUpperBound() * specs.emitRate->getEmitRate() * 1.5f
    + 1.f);

  if (maxParticleCount > m_maxParticleCount)
  {
    // Setup new particle states

    size_t singleStatesSize = specs.computeStatesRequiredSize();
    size_t bufferSize       = singleStatesSize * maxParticleCount;

    m_particleStatesBuffer.ensureCapacity(bufferSize, true);
    m_instanceDataBuffer.ensureCapacity(
      sizeof(RawInstanceData) * maxParticleCount, false);

    std::span<std::byte> mapped =
      m_particleStatesBuffer.mapWriteOnly<std::byte>(
        m_maxParticleCount * singleStatesSize,
        (maxParticleCount - m_maxParticleCount) * singleStatesSize, false);

    for (size_t i = 0; i < maxParticleCount - m_maxParticleCount; ++i)
    {
      /// @todo Make this safer, particle states should be using
      /// render::StructuredBuffer class

      std::byte* currentStates = (mapped.data() + i * singleStatesSize);
      // Alive attrib must be the first one
      glm::uint& currentAlive = *reinterpret_cast<glm::uint*>(currentStates);
      currentAlive            = 0;

      // The rest of the data is irrelevant since the particle is not alive
    }

    m_particleStatesBuffer.unmap();
  }

  m_maxParticleCount = maxParticleCount;
}

void ParticleEmitterRender::_updateEmitterData(const ParticleEmitter& emitter)
{
  const ParticleEmitter::Specs& specs = emitter.getSpecs();

  specs.lifeTime->assignStructuredBufferData(m_rawEmitterSpecs);
  specs.emitRate->assignStructuredBufferData(m_rawEmitterSpecs);
  specs.spawnPosition->assignStructuredBufferData(m_rawEmitterSpecs);
  specs.spawnVelocity->assignStructuredBufferData(m_rawEmitterSpecs);
  specs.spawnScale->assignStructuredBufferData(m_rawEmitterSpecs);
  specs.spawnColor->assignStructuredBufferData(m_rawEmitterSpecs);

  m_rawEmitterSpecs.uploadTo(m_emitterDataBuffer);
}

void ParticleEmitterRender::_executeUpdateParticles(
  const ParticleEmitter& emitter)
{
  const gl::Shader& updaterShader = m_updaterMaterial.getShader();
  updaterShader.bind();
  _bindUpdaterShaderData(updaterShader);
  updaterShader.setUniform1ui("u_maxParticleCount", m_maxParticleCount);
  updaterShader.setUniform1f("u_deltaTime",
                             Application::Get().getDeltaTime().seconds());
  updaterShader.setUniform1ui("u_particlesToSpawn",
                              emitter.getNextParticleCountToSpawn());
  updaterShader.setUniform1f("u_firstParticleStamp",
                             emitter.getNextParticleToSpawnStartingLifetime());

  {
    const auto now = std::chrono::high_resolution_clock::now();

    const auto nanosecs = std::chrono::duration_cast<std::chrono::nanoseconds>(
                            now.time_since_epoch())
                            .count();
    const uint32_t nanosecsUint32 = static_cast<uint32_t>(
      nanosecs % std::numeric_limits<decltype(nanosecs)>::max());

    updaterShader.setUniform1ui("u_randomSeed", nanosecsUint32);
  }

  glm::uvec3 dispatch = gl::Shader::ComputeDispatchSize(
    s_updaterGroupSize, glm::uvec3(m_maxParticleCount, 1, 1));
  GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));
}

void ParticleEmitterRender::_executeRenderParticles(
  const RenderPassContext& ctx, const glm::mat4* model)
{
  if (!m_mesh.isValid()) return;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const RenderMesh& mesh   = m_mesh->getSubMeshes().at(0).renderMesh;
  const gl::Shader& shader = m_renderMaterial.getShader();

  gl::VertexArray::Bind(mesh.getVertexArray());
  gl::Buffer::Bind(mesh.getIndexBuffer(), GL_ELEMENT_ARRAY_BUFFER);
  gl::Buffer::Bind(m_indirectCommandsBuffer.getBuffer(),
                   GL_DRAW_INDIRECT_BUFFER);
  shader.bind();
  _bindParticleInstanceData(shader);

  glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

  for (const render::View& view : ctx.views)
  {
    const auto& vpOrigin = view.getViewport().getOrigin();
    const auto& vpSize   = view.getViewport().getSize();
    glViewport(vpOrigin.x, vpOrigin.y, vpSize.x, vpSize.y);
    shader.setUniformMat4f("u_model", *model);
    shader.setUniformMat4f("u_viewProj", view.getCamera()->getViewProjection());

    GLCall(glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr));
  }

  glDisable(GL_BLEND);
}

void ParticleEmitterRender::_bindUpdaterShaderData(
  const gl::Shader& shader) const
{
  _bindEmittersData(shader);
  _bindIndirectCommands(shader);
  _bindParticleStates(shader);
  _bindParticleInstanceData(shader);
  _bindSpawnData(shader);
}

void ParticleEmitterRender::_bindRenderShaderData(
  const gl::Shader& shader) const
{
  _bindParticleInstanceData(shader);
}

void ParticleEmitterRender::_bindEmittersData(const gl::Shader& shader) const
{
  shader.setStorageBuffer("EmittersDataBlock", m_emitterDataBuffer.getBuffer());
}

void ParticleEmitterRender::_bindSpawnData(const gl::Shader& shader) const
{
  shader.setStorageBuffer("EmitterSpawnDataBlock",
                          m_spawnDataBuffer.getBuffer());
}

void ParticleEmitterRender::_bindIndirectCommands(
  const gl::Shader& shader) const
{
  shader.setStorageBuffer("IndirectCommandsBlock",
                          m_indirectCommandsBuffer.getBuffer());
}

void ParticleEmitterRender::_bindParticleStates(const gl::Shader& shader) const
{
  shader.setStorageBuffer("ParticleStatesBlock",
                          m_particleStatesBuffer.getBuffer());
}

void ParticleEmitterRender::_bindParticleInstanceData(
  const gl::Shader& shader) const
{
  shader.setStorageBuffer("ParticleInstanceDataBlock",
                          m_instanceDataBuffer.getBuffer());
}
