#include "Vroom/Render/ParticleEmitterRender.h"
#include <variant>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/RenderView.h"
#include "glm/glm.hpp"

using namespace vrm;

namespace
{

struct RawParticleStates
{
  glm::vec4 spawnPosition;
  glm::vec4 deathPosition;
  glm::vec4 spawnColor;
  glm::vec4 deathColor;
  glm::vec4 spawnScale;
  glm::vec3 deathScale;

  glm::uint alive            = 0;
  float     ellapsedLifeTime = 0.f;
  float     maxLifeTime      = 0.f;
  float     _pad[2];
};

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
  m_emitterDataBuffer.ensureCapacity(sizeof(RawParticleEmitterSpecs));
  m_spawnDataBuffer.ensureCapacity(sizeof(RawEmitterSpawnData));
  m_indirectCommandsBuffer.ensureCapacity(
    sizeof(RawDrawElementsIndirectCommand));
}

ParticleEmitterRender::~ParticleEmitterRender() {}

void ParticleEmitterRender::setMesh(MeshAsset::Handle mesh)
{
  m_mesh = mesh;

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
  ::RawParticleEmitterSpecs::Layout layout;
  // std::vector<std::string>        defines;
  // std::string                     currentDefine;

  // const auto visitor = overloads{
  //   [&](const ConstParticleEmitterField& field)
  //   {
  //     defines.emplace_back(currentDefine + "_CONST_FIELD");
  //   },
  //   [&](const RandomRangeEmitterField& field)
  //   {
  //     defines.emplace_back(currentDefine + "_RANDOM_RANGE_FIELD");
  //   },
  // };

  // ParticleEmitterAttribute attribute = emitter.getSpecs().position;

  // const std::string attributeDefine =
  //   "VRM_PARTICLE_" + attribute.getShaderDefineName();

  // currentDefine = attributeDefine + "_SPAWN";
  // std::visit(visitor, attribute.spawnValue);

  // currentDefine = attributeDefine + "_DEATH";
  // std::visit(visitor, attribute.deathValue);

  // layout.addAttribute(::RawParticleEmitterSpecs::EAttributeName::ePosition,
  // 2); layout.addAttribute(::RawParticleEmitterSpecs::EAttributeName::eColor,
  // 2); layout.addAttribute(::RawParticleEmitterSpecs::EAttributeName::eScale,
  // 2);

  _setupUpdaterMaterial(layout);
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
  const vrm::RawParticleEmitterSpecs::Layout& layout)
{
  MaterialAsset::Handle updaterAsset =
    AssetManager::Get().tryGetAsset<MaterialAsset>(
      "Resources/Engine/Material/UpdateParticlesMaterial.json");
  VRM_CHECK_RET(updaterAsset.isValid());

  MaterialDefines defines;
  defines.add("VRM_UPDATER_GROUP_SIZE", s_updaterGroupSize.x);

  m_updaterMaterial.setMaterialAsset(updaterAsset);
  m_updaterMaterial.prepare(defines);
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
  glm::uint   maxParticleCount =
    static_cast<glm::uint>(specs.lifeTime * specs.emitRate * 1.5f + 1.f);

  m_particleStatesBuffer.ensureCapacity(
    sizeof(RawParticleStates) * maxParticleCount, true);
  m_instanceDataBuffer.ensureCapacity(
    sizeof(RawInstanceData) * maxParticleCount, false);

  if (maxParticleCount > m_maxParticleCount)
  {
    // Setup new particle states

    std::span<RawParticleStates> mapped =
      m_particleStatesBuffer.mapWriteOnly<RawParticleStates>(
        m_maxParticleCount * sizeof(RawParticleStates),
        (maxParticleCount - m_maxParticleCount) * sizeof(RawParticleStates),
        false);

    for (RawParticleStates& states : mapped)
    {
      states.alive = 0; // So it can be spawned
      // The rest of the data is irrelevant since the particle is not alive
    }

    m_particleStatesBuffer.unmap();
  }

  m_maxParticleCount = maxParticleCount;
}

template <glm::length_t Dim>
static void SetRawSpecField(glm::vec<Dim, float>&           dst,
                            const ParticleEmitterFieldType& src)
{
  if (std::holds_alternative<ConstParticleEmitterField>(src))
  {
    dst = glm::vec<Dim, float>(std::get<ConstParticleEmitterField>(src).value);
  }
}

void ParticleEmitterRender::_updateEmitterData(const ParticleEmitter& emitter)
{
  std::span<RawParticleEmitterSpecs> mapped =
    m_emitterDataBuffer.mapWriteOnly<RawParticleEmitterSpecs>(
      0, sizeof(RawParticleEmitterSpecs), true);

  RawParticleEmitterSpecs&      rawSpecs = mapped[0];
  const ParticleEmitter::Specs& specs    = emitter.getSpecs();

  rawSpecs.lifeTime = specs.lifeTime;
  rawSpecs.emitRate = specs.emitRate;

  SetRawSpecField(rawSpecs.spawnColor, specs.color.spawnValue);
  SetRawSpecField(rawSpecs.deathColor, specs.color.deathValue);

  SetRawSpecField(rawSpecs.spawnPosition, specs.position.spawnValue);
  SetRawSpecField(rawSpecs.deathPosition, specs.position.deathValue);

  SetRawSpecField(rawSpecs.spawnScale, specs.scale.spawnValue);
  SetRawSpecField(rawSpecs.deathScale, specs.scale.deathValue);

  m_emitterDataBuffer.unmap();
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

  glm::uvec3 dispatch = gl::Shader::ComputeDispatchSize(
    s_updaterGroupSize, glm::uvec3(m_maxParticleCount, 1, 1));
  GLCall(glDispatchCompute(dispatch.x, dispatch.y, dispatch.z));
}

void ParticleEmitterRender::_executeRenderParticles(
  const RenderPassContext& ctx, const glm::mat4* model)
{
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
