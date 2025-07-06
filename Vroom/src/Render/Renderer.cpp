#include "Vroom/Render/Renderer.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Core/Application.h"

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/Passes/DrawScenePass.h"
#include "Vroom/Render/Passes/BlitFrameBufferPass.h"
#include "Vroom/Render/Passes/ClearFrameBufferPass.h"
#include "Vroom/Render/Passes/LightClusteringPass.h"
#include "Vroom/Render/Passes/ShadowMappingPass.h"
#include "Vroom/Render/Passes/ClearTexturePass.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Camera/CameraBasic.h"

#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/ShaderAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"

#include "Vroom/Scene/Scene.h"

using namespace vrm;
using namespace vrm::gl;

std::unique_ptr<Renderer> Renderer::s_Instance = nullptr;

Renderer::Renderer()
{
  m_gpuFeatures.query();
}

Renderer::~Renderer()
{
}

void Renderer::setRenderSettings(const RenderSettings& settings)
{
  if (m_renderSettings != settings)
  {
    m_renderSettings = settings;
    m_passManagerDirty = true;
  }
}

void Renderer::Init()
{
  VRM_ASSERT_MSG(s_Instance == nullptr, "Renderer already initialized.");
  s_Instance.reset(new Renderer());
}

void Renderer::Shutdown()
{
  s_Instance.reset();
}

Renderer& Renderer::Get()
{
  VRM_ASSERT_MSG(s_Instance != nullptr, "Renderer not initialized.");
  return *s_Instance;
}

void Renderer::createRenderPasses()
{
  m_passManager.reset();
  m_frameBufferPool.clear();
  m_texturePool.clear();
  m_buffersPool.clear();
  m_autoBuffersPool.clear();

  m_mainFrameBuffer.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
  m_mainFrameBuffer.setColorAttachment(0, 4, glm::vec4 { 0.1f, 0.1f, 0.1f, 1.f });
  m_mainFrameBuffer.setRenderBufferDepthAttachment();
  VRM_ASSERT_MSG(m_mainFrameBuffer.validate(), "Could not build main framebuffer");

  auto aa = m_renderSettings.antiAliasingLevel;
  bool aaOK = (aa != 0 && ((aa & (aa - 1)) == 0));
  VRM_ASSERT_MSG(aaOK, "Invalid antialiasing value: {}", aa);

  // MSAA
  if (aa > 1)
  {
    auto& fb = *m_frameBufferPool.emplace<gl::OwningFrameBuffer>("MsaaFramebuffer");
    fb.create(m_viewport.getSize().x, m_viewport.getSize().y, aa);
    fb.setColorAttachment(0, 4, glm::vec4 { 0.1f, 0.1f, 0.1f, 1.f });
    fb.setRenderBufferDepthAttachment(1.f);
    VRM_ASSERT_MSG(fb.validate(), "Could not build MSAA framebuffer");

    m_renderFrameBuffer = &fb;
  }
  else
  {
    m_renderFrameBuffer = &m_mainFrameBuffer;
  }

  // Clearing render framebuffer
  // Useful to do it before everything because we could draw things at any pass
  {
    auto& pass = m_passManager.pushPass<ClearFrameBufferPass>();

    pass.framebuffer = m_renderFrameBuffer;
  }

  // Shadow mapping
  if (m_renderSettings.shadowsEnable)
  {
    auto& maps = *m_texturePool.emplace("DirLightsShadowMaps");

    auto& pass = m_passManager.pushPass<ShadowMappingPass>();

    pass.lights = &m_LightRegistry;
    pass.meshRegistry = &m_meshRegistry;
    pass.resolution = 4096;
    pass.depthTextureArray = &maps;
    pass.lightMatricesBuffer = m_autoBuffersPool.emplace("LightMatricesStorageBuffer");
  }

  // Light clustering
  if (m_renderSettings.clusteredShading)
  {
    auto& pass = m_passManager.pushPass<LightClusteringPass>();
    pass.camera = &m_Camera;
    pass.clusterCount = m_renderSettings.clusterCount;
    pass.lightsStorageBuffer = &m_LightRegistry.getPointLightsStorageBuffer();
    pass.clusteredLights = &m_ClusteredLights;
  }

  // Main scene rendering
  {
    auto& pass = m_passManager.pushPass<DrawSceneRenderPass>();
    pass.meshTags.set(EMeshTag::eVisible);
    pass.meshRegistry = &m_meshRegistry;
    pass.framebufferTarget = m_renderFrameBuffer;
    pass.viewport = &m_viewport;
    pass.faceCulling = DrawSceneRenderPass::EFaceCulling::eCullBack;
    pass.frontFace = DrawSceneRenderPass::EFrontFace::eCCW;
    pass.storageBufferParameters["PointLightBlock"] = &m_LightRegistry.getPointLightsStorageBuffer();
    pass.storageBufferParameters["DirLightBlock"] = &m_LightRegistry.getDirLightsStorageBuffer();
    if (m_renderSettings.clusteredShading)
    {
      pass.addDefine("VRM_CLUSTERED_RENDERING");
      pass.storageBufferParameters["ClusterInfoBlock"] = &m_ClusteredLights.getClustersShaderStorage();
    }
    if (m_renderSettings.showLightComplexity)
      pass.addDefine("VRM_LIGHT_COMPLEXITY");
    
    pass.shadowsEnable = m_renderSettings.shadowsEnable;
    if (m_renderSettings.shadowsEnable)
    {
      pass.dirLightShadowMaps = m_texturePool.get("DirLightsShadowMaps");
      pass.storageBufferParameters["LightMatricesBlock"] = &m_autoBuffersPool.get("LightMatricesStorageBuffer")->getBuffer();
      pass.softShadowKernelRadius = m_renderSettings.softShadowKernelRadius;
    }
  }

  // Entity picking
  {
    auto& tex = *m_texturePool.emplace("PickingColorTexture");
    gl::Texture::Desc desc;
    {
      desc.dimension = 2;
      desc.width = m_viewport.getSize().x;
      desc.height = m_viewport.getSize().y;
      desc.internalFormat = GL_R32UI;
      desc.format = GL_UNSIGNED_INT;
    }
    tex.create(desc);
    
    auto& depth = *m_texturePool.emplace("PickingDepthTexture");
    {
      desc.internalFormat = GL_DEPTH_COMPONENT24;
      desc.format = GL_DEPTH_COMPONENT;
    }
    depth.create(desc);
    
    auto& fb = *m_frameBufferPool.emplace("PickingFrameBuffer");
    fb.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
    fb.setColorAttachment(0, tex);
    fb.setDepthAttachment(depth);

    VRM_ASSERT_MSG(fb.validate(), "Could not build picking framebuffer");

    {
      auto& pass = m_passManager.pushPass<ClearFrameBufferPass>();
      pass.framebuffer = &fb;
    }

    {
      auto& pass = m_passManager.pushPass<DrawSceneRenderPass>();
      pass.addDefine("VRM_ENTITY_PICKING");
      pass.meshTags.set(EMeshTag::eVisible);
      pass.meshRegistry = &m_meshRegistry;
      pass.framebufferTarget = &fb;
      pass.viewport = &m_viewport;
      pass.faceCulling = DrawSceneRenderPass::EFaceCulling::eCullBack;
      pass.frontFace = DrawSceneRenderPass::EFrontFace::eCCW;
      pass.shadowsEnable = false;
    }
  }

  // MSAA
  if (aa > 1)
  {
    auto& pass = m_passManager.pushPass<BlitFrameBufferPass>();
    pass.source = m_renderFrameBuffer;
    pass.destination = &m_mainFrameBuffer;
  }

  m_passManager.init();
  m_passManagerDirty = false;
}

void Renderer::beginScene(const CameraBasic& camera)
{
  if (m_passManagerDirty)
  {
    Application::Get().setFrameRateLimit(m_renderSettings.frameRateLimit);
    createRenderPasses();
  }

  m_Camera = &camera;

  m_meshRegistry.startRegistering();
  m_LightRegistry.startRegistering();
}

void Renderer::endScene()
{
  m_meshRegistry.endRegistering();
  m_LightRegistry.endRegistering();

  RenderPassContext renderContext;
  renderContext.mainCamera = m_Camera;
  renderContext.frameBufferTarget = m_renderFrameBuffer;
  renderContext.viewport = m_viewport;

  // RenderPass setup stage
  m_passManager.setup(renderContext);

  // RenderPass render/cleanup stages
  m_passManager.render(renderContext);
  m_passManager.cleanup(renderContext);

  // Clearing data for next frame
  m_Camera = nullptr;
}

void Renderer::submitMesh(uint32_t id, const MeshComponent& meshComponent, const glm::mat4* model)
{
  uint32_t i = 0;
  for (const auto& submesh : meshComponent.getMesh()->getSubMeshes())
  {
    MeshRenderInfo info;
    info.renderMeshId = (((size_t)id) << 32) | i; // Id is tracking the mesh component + its submesh
    info.mesh = &submesh.renderMesh;
    info.material = meshComponent.getMaterials().getMaterial(i);
    info.model = model;
    info.tags.set(EMeshTag::eVisible, meshComponent.isVisible());
    info.tags.set(EMeshTag::eShadowCaster, meshComponent.doesCastShadow());
    info.entityId = id;

    m_meshRegistry.submit(info.renderMeshId, std::move(info));

    ++i;
  }
}

void Renderer::submitPointLight(size_t id, const PointLightComponent& pointLight, const glm::vec3& position)
{
  m_LightRegistry.submitLight(pointLight, position, id);
}

void Renderer::submitDirectionalLight(size_t id, const DirectionalLightComponent& dirLight, const glm::vec3& direction)
{
  m_LightRegistry.submitLight(dirLight, direction, id);
}

const glm::uvec2& Renderer::getViewportOrigin() const
{
  return m_viewport.getOrigin();
}

const glm::uvec2& Renderer::getViewportSize() const
{
  return m_viewport.getSize();
}

void Renderer::setViewport(const glm::uvec2& o, const glm::uvec2& s)
{
  setViewportOrigin(o);
  setViewportSize(s);
}

void Renderer::setViewportOrigin(const glm::uvec2& o)
{
  m_viewport.setOrigin(o);
}

void Renderer::setViewportSize(const glm::uvec2& s)
{
  m_viewport.setSize(s);
  m_passManagerDirty = true;
}

uint32_t Renderer::getEntityIndexOnPixel(const glm::ivec2& px) const
{
  if (!m_frameBufferPool.contains("PickingFrameBuffer"))
  {
    return 0;
  }
  
  const auto& fb = *m_frameBufferPool.get("PickingFrameBuffer");
  fb.bind();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  uint32_t pixel;
  glReadPixels(px.x, px.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
  glReadBuffer(GL_NONE);
  return pixel;
}
