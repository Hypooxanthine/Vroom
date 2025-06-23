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
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Abstraction/ArrayTexture2D.h"
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
  m_arrayTexture2DPool.clear();
  m_storageBufferPool.clear();
  m_autoresizeStorageBufferPool.clear();

  m_mainFrameBuffer.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
  m_mainFrameBuffer.setColorAttachment(0, 4, glm::vec4 { 0.1f, 0.1f, 0.1f, 1.f });
  m_mainFrameBuffer.setRenderBufferDepthAttachment();
  VRM_ASSERT_MSG(m_mainFrameBuffer.validate(), "Could not build main framebuffer");

  // Entity picking
  { 
    if (m_pickingTexture)
    {
      glDeleteTextures(1, &m_pickingTexture);
      m_pickingTexture = 0;
    }

    glGenTextures(1, &m_pickingTexture);
    VRM_ASSERT_MSG(m_pickingTexture, "Could not create picking texture");
    glBindTexture(GL_TEXTURE_2D, m_pickingTexture);

    GLenum internalFormat = GL_R32UI;
    GLenum format = GL_RED_INTEGER;
    GLenum type = GL_UNSIGNED_INT;
    static const GLuint clearValue = 0;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_viewport.getSize().x, m_viewport.getSize().y, 0, format, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto& pass = m_passManager.pushPass<ClearTexturePass>();
    pass.texture = m_pickingTexture;
    pass.format = format;
    pass.type = type;
    pass.clearValue = &clearValue;
    pass.width = m_viewport.getSize().x;
    pass.height = m_viewport.getSize().y;
  }

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
    auto& maps = *m_arrayTexture2DPool.emplace("DirLightsShadowMaps");

    auto& pass = m_passManager.pushPass<ShadowMappingPass>();

    pass.lights = &m_LightRegistry;
    pass.meshRegistry = &m_meshRegistry;
    pass.resolution = 4096;
    pass.depthTextures = &maps;
    pass.lightMatricesStorageBuffer = m_autoresizeStorageBufferPool.emplace("lightMatricesStorageBuffer");
  }

  // Light clustering
  {
    auto& pass = m_passManager.pushPass<LightClusteringPass>();
    pass.camera = &m_Camera;
    pass.clusterCount = { 12, 12, 24 };
    pass.lightsStorageBuffer = &m_LightRegistry.getPointLightsStorageBuffer();
    pass.clusteredLights = &m_ClusteredLights;
  }

  // Main scene rendering
  {
    auto& pass = m_passManager.pushPass<DrawSceneRenderPass>();
    pass.meshRegistry = &m_meshRegistry;
    pass.framebufferTarget = m_renderFrameBuffer;
    pass.viewport = &m_viewport;
    pass.faceCulling = DrawSceneRenderPass::EFaceCulling::eCullBack;
    pass.frontFace = DrawSceneRenderPass::EFrontFace::eCCW;
    pass.storageBufferParameters["LightBlock"] = &m_LightRegistry.getPointLightsStorageBuffer();
    pass.storageBufferParameters["ClusterInfoBlock"] = &m_ClusteredLights.getClustersShaderStorage();
    
    pass.shadowsEnable = m_renderSettings.shadowsEnable;
    if (m_renderSettings.shadowsEnable)
    {
      pass.dirLightShadowMaps = m_arrayTexture2DPool.get("DirLightsShadowMaps");
      pass.storageBufferParameters["LightMatricesBlock"] = m_autoresizeStorageBufferPool.get("lightMatricesStorageBuffer");
      pass.softShadowKernelRadius = m_renderSettings.softShadowKernelRadius;
    }
    pass.entityPickingTex = m_pickingTexture;
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
    info.mesh = &submesh.renderMesh;
    info.material = meshComponent.getMaterials().getMaterial(i);
    info.model = model;
    info.visible = meshComponent.isVisible();
    info.castsShadow = meshComponent.doesCastShadow();
    info.entityId = id;
    size_t subMeshId = id;
    subMeshId <<= 32;
    subMeshId |= i;

    m_meshRegistry.submit(subMeshId, std::move(info));

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
  GLuint pixel;
  glGetTextureSubImage(m_pickingTexture, 0, px.x, px.y, 0, 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, sizeof(pixel), &pixel);
  return pixel;
}
