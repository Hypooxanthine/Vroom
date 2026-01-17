#include "Vroom/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Render/GPURuntimeFeatures.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/ParticleEmitterRegistry.h"
#include "Vroom/Render/Passes/LightClusteringPass.h"
#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/Passes/RenderSkyboxPass.h"
#include "Vroom/Render/Passes/ShadowMappingPass.h"
#include "Vroom/Render/RenderPipeline.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "Vroom/Scene/Scene.h"

using namespace vrm;
using namespace vrm::gl;

GPURuntimeFeatures Renderer::s_gpuFeatures;

Renderer::Renderer()
{
  if (!s_gpuFeatures.wasQueried())
    s_gpuFeatures.query();

  RenderPipelineContext ctx;
  ctx.lights           = &m_LightRegistry;
  ctx.meshes           = &m_meshRegistry;
  ctx.skybox           = &m_skybox;
  ctx.particleEmitters = &m_particleEmitterRegistry;

  m_pipeline.setContext(ctx);
}

Renderer::~Renderer()
{}

void Renderer::beginScene(const RenderLayout* layout)
{
  m_pipeline.generateIfDirty();

  m_renderLayout = layout;

  m_meshRegistry.startRegistering();
  m_LightRegistry.startRegistering();
  m_particleEmitterRegistry.startRegistering();
  m_skybox.clear();
}

void Renderer::endScene()
{
  m_meshRegistry.endRegistering();
  m_LightRegistry.endRegistering();
  m_particleEmitterRegistry.endRegistering();

  RenderPassContext renderContext;
  for (size_t row = 0; row < m_renderLayout->getRows(); ++row)
    for (size_t col = 0; col < m_renderLayout->getCols(); ++col)
      if (m_renderLayout->isViewUsed(row, col))
        renderContext.views.emplace_back(m_renderLayout->getView(row, col), m_frameSize);

  if (renderContext.views.size() > 0)
    m_pipeline.execute(renderContext);

  // Clearing data for next frame
  m_renderLayout = nullptr;
}

void Renderer::submitMesh(uint32_t id, const MeshComponent& meshComponent, const glm::mat4* model)
{
  uint32_t i = 0;
  for (const auto& submesh : meshComponent.getMesh()->getSubMeshes())
  {
    MeshRenderInfo info;
    info.renderMeshId = (((size_t)id) << 32) | i; // Id is tracking the mesh component + its submesh
    info.mesh         = &submesh.renderMesh;
    info.material     = meshComponent.getMaterials().getMaterial(i);
    info.model        = model;
    info.tags.set(EMeshTag::eVisible, meshComponent.isVisible());
    info.tags.set(EMeshTag::eShadowCaster, meshComponent.doesCastShadow());
    info.entityId = id;

    m_meshRegistry.submit(info.renderMeshId, std::move(info));

    ++i;
  }
}

void Renderer::submitSkybox(const SkyboxComponent& skyboxComponent)
{
  m_skybox.setCubemap(skyboxComponent.getCubemapAsset());
}

void Renderer::submitPointLight(size_t id, const PointLightComponent& pointLight, const glm::vec3& position)
{
  m_LightRegistry.submitLight(pointLight, position, id);
}

void Renderer::submitDirectionalLight(size_t id, const DirectionalLightComponent& dirLight, const glm::vec3& direction)
{
  m_LightRegistry.submitLight(dirLight, direction, id);
}

void Renderer::submitParticleSystem(uint32_t entityId, const ParticleSystemComponent& system, const glm::mat4* model)
{
  const auto& emitters = system.getEmitters();

  for (size_t i = 0; i < emitters.size(); ++i)
  {
    const ParticleEmitter* emitter = &emitters[i];
    size_t                 id      = ((size_t)entityId << 32) | i;

    ParticleSystemRenderInfo info;
    info.emitter = emitter;
    info.model   = model;
    m_particleEmitterRegistry.submit(id, info);
  }
}

void Renderer::setFrameSize(const glm::uvec2& s)
{
  if (m_frameSize != s)
  {
    m_frameSize = s;
    m_pipeline.getRendererAttorney().setFrameSize(s);
  }
}
