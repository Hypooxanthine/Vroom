#pragma once

#include <glm/glm.hpp>

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/AutoBuffer.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"

namespace vrm
{

class ParticleEmitter;
class PassMaterial;
class RenderPassContext;

class ParticleEmitterRender
{
public:

  ParticleEmitterRender();
  ~ParticleEmitterRender();

  ParticleEmitterRender(const ParticleEmitterRender& other)            = delete;
  ParticleEmitterRender& operator=(const ParticleEmitterRender& other) = delete;

  ParticleEmitterRender(ParticleEmitterRender&& other)            = default;
  ParticleEmitterRender& operator=(ParticleEmitterRender&& other) = default;

  void setMesh(MeshAsset::Handle mesh);
  void setRenderMaterial(MaterialAsset::Handle material);

  void rebuildMaterials(const ParticleEmitter& emitter);
  void updateResources(const ParticleEmitter& emitter);
  void prepareFrame(const ParticleEmitter& emitter);
  void executeRender(const ParticleEmitter&   emitter,
                     const RenderPassContext& ctx, const glm::mat4* model);

private:

  struct RawDrawElementsIndirectCommand
  {
    glm::uint count         = 0;
    glm::uint instanceCount = 0;
    glm::uint firstIndex    = 0;
    int       baseVertex    = 0;
    glm::uint baseInstance  = 0;
  };

private:

  void _uploadSpawnData(const ParticleEmitter& emitter) const;
  void _uploadIndirectCommandData() const;

  void _updateParticleStates(const ParticleEmitter& emitter);
  void _updateEmitterData(const ParticleEmitter& emitter);

  void _executeUpdateParticles(const ParticleEmitter& emitter);
  void _executeRenderParticles(const RenderPassContext& ctx,
                               const glm::mat4*         model);

  void _bindUpdaterShaderData(const gl::Shader& shader) const;
  void _bindRenderShaderData(const gl::Shader& shader) const;
  void _bindEmittersData(const gl::Shader& shader) const;
  void _bindSpawnData(const gl::Shader& shader) const;
  void _bindIndirectCommands(const gl::Shader& shader) const;
  void _bindParticleStates(const gl::Shader& shader) const;
  void _bindParticleInstanceData(const gl::Shader& shader) const;

private:

  constexpr static glm::uvec3 s_updaterGroupSize = { 64, 1, 1 };

  MeshAsset::Handle m_mesh;

  PassMaterial m_updaterMaterial;
  PassMaterial m_renderMaterial;

  render::StructuredBuffer       m_rawEmitterSpecs;
  render::SSBO430Layout          m_rawEmitterSpecsLayout;
  RawDrawElementsIndirectCommand m_indirectCommand;

  render::AutoBuffer m_emitterDataBuffer;
  render::AutoBuffer m_spawnDataBuffer;
  render::AutoBuffer m_indirectCommandsBuffer;
  render::AutoBuffer m_particleStatesBuffer;
  render::AutoBuffer m_instanceDataBuffer;

  size_t m_maxParticleCount = 0;
};

} // namespace vrm
