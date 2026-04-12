#include "Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderObjects/PointLight.h"
#include "Renderer/GPURuntimeFeatures.h"
#include "Renderer/LightClusteringPass.h"
#include "Renderer/ParticleEmitter.h"
#include "Renderer/ParticleEmitterRegistry.h"
#include "Renderer/RenderLayout.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/ShadowMappingPass.h"

using namespace vrm;
using namespace vrm::gl;

namespace
{

static GPURuntimeFeatures s_gpuFeatures = {};

}

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

const GPURuntimeFeatures& Renderer::getGPUFeatures() const
{
  return s_gpuFeatures;
}

void Renderer::beginScene(const RenderLayout* layout)
{
  m_pipeline.generateIfDirty();

  m_renderLayout = layout;

  m_meshRegistry.startRegistering();
  m_LightRegistry.startRegistering();
  m_particleEmitterRegistry.startRegistering();
  m_skybox = nullptr;
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

void Renderer::submitMesh(const MeshRenderInfo& meshInfo)
{
  // uint32_t i = 0;
  // for (const auto& submesh : mesh->getSubMeshes())
  // {
  //   MeshRenderInfo info;
  //   info.renderMeshId = (((size_t)id) << 32) | i; // Id is tracking the mesh component + its submesh
  //   info.mesh         = &submesh.renderMesh;
  //   info.material     = meshComponent.getMaterials().getMaterial(i);
  //   info.model        = model;
  //   info.tags.set(EMeshTag::eVisible, meshComponent.isVisible());
  //   info.tags.set(EMeshTag::eShadowCaster, meshComponent.doesCastShadow());
  //   info.entityId = id;

  //   m_meshRegistry.submit(info.renderMeshId, std::move(info));

  //   ++i;
  // }

  m_meshRegistry.submit(meshInfo.renderMeshId, meshInfo);
}

void Renderer::submitSkybox(const render::Cubemap& skybox)
{
  m_skybox = &skybox;
}

void Renderer::submitPointLight(size_t id, const render::PointLight& pointLight)
{
  m_LightRegistry.submitLight(pointLight, id);
}

void Renderer::submitDirectionalLight(size_t id, const render::DirectionalLight& dirLight)
{
  m_LightRegistry.submitLight(dirLight, id);
}

void Renderer::submitParticleEmitter(uint32_t id, const ParticleSystemRenderInfo& emitter)
{
  // const auto& emitters = system.getEmitters();

  // for (size_t i = 0; i < emitters.size(); ++i)
  // {
  //   const ParticleEmitter* emitter = &emitters[i];
  //   size_t                 id      = ((size_t)entityId << 32) | i;

  //   ParticleSystemRenderInfo info;
  //   info.emitter = emitter;
  //   info.model   = model;
  //   m_particleEmitterRegistry.submit(id, info);
  // }

  m_particleEmitterRegistry.submit(id, emitter);
}

void Renderer::setFrameSize(const glm::uvec2& s)
{
  if (m_frameSize != s)
  {
    m_frameSize = s;
    m_pipeline.getRendererAttorney().setFrameSize(s);
  }
}
