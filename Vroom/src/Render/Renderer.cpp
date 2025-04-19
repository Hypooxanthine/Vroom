#include "Vroom/Render/Renderer.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Core/Application.h"

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/Passes/DrawScenePass.h"
#include "Vroom/Render/Passes/BlitFrameBufferPass.h"
#include "Vroom/Render/Passes/LightClusteringPass.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
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
  m_mainFrameBuffer.create(1, 1, 1);
  m_mainFrameBuffer.setColorAttachment(0, 4, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.f });
  m_mainFrameBuffer.setRenderBufferDepthAttachment();
  VRM_ASSERT_MSG(m_mainFrameBuffer.validate(), "Could not build main framebuffer");
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

Renderer &Renderer::Get()
{
  VRM_ASSERT_MSG(s_Instance != nullptr, "Renderer not initialized.");
  return *s_Instance;
}

void Renderer::createRenderPasses()
{
  m_passManager.reset();
  m_frameBufferPool.clear();

  auto aa = m_renderSettings.antiAliasingLevel;
  bool aaOK = (aa != 0 && ((aa & (aa - 1)) == 0));
  VRM_ASSERT_MSG(aaOK, "Invalid antialiasing value: {}", aa);

  // Shadow mapping
  {
    // auto& fb = m_frameBufferPool.emplace_back();
    // fb = std::make_unique<gl::FrameBuffer>();
    // fb->create(m_ViewportSize.x, m_ViewportSize.y, 1);
    // fb->setDepthAttachment()
    // VRM_ASSERT_MSG(fb->validate(), "Could not build shadow mapping framebuffer");
  }

  gl::FrameBuffer *renderFrameBuffer = nullptr;

  // MSAA
  if (aa > 1)
  {
    auto fb = std::make_unique<gl::OwningFrameBuffer>();
    fb->create(m_ViewportSize.x, m_ViewportSize.y, aa);
    fb->setColorAttachment(0, 4, glm::vec4{ 0.1f, 0.1f, 0.1f, 1.f });
    fb->setRenderBufferDepthAttachment(1.f);
    VRM_ASSERT_MSG(fb->validate(), "Could not build MSAA framebuffer");

    renderFrameBuffer = fb.get();
    
    m_frameBufferPool.emplace_back(std::move(fb));
  }
  else
  {
    renderFrameBuffer = &m_mainFrameBuffer;
  }
  
  m_mainFrameBuffer.resize(m_ViewportSize.x, m_ViewportSize.y);

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
    pass.framebufferTarget = renderFrameBuffer;
    pass.camera = &m_Camera;
    pass.viewportOrigin = &m_ViewportOrigin;
    pass.viewportSize = &m_ViewportSize;
    pass.faceCulling = DrawSceneRenderPass::EFaceCulling::eCullBack;
    pass.frontFace = DrawSceneRenderPass::EFrontFace::eCCW;
    pass.storageBufferParameters["LightBlock"] = &m_LightRegistry.getPointLightsStorageBuffer();
    pass.storageBufferParameters["ClusterInfoBlock"] = &m_ClusteredLights.getClustersShaderStorage();
  }

  // MSAA
  if (aa > 1)
  {
    auto& pass = m_passManager.pushPass<BlitFrameBufferPass>();
    pass.source = renderFrameBuffer;
    pass.destination = &m_mainFrameBuffer;
  }

  m_passManager.init();
  m_passManagerDirty = false;
}

void Renderer::beginScene(const CameraBasic &camera)
{
  if (m_passManagerDirty)
  {
    createRenderPasses();
  }

  m_Camera = &camera;

  m_meshRegistry.startRegistering();
}

void Renderer::endScene()
{
  m_meshRegistry.endRegistering();

  // Setting up lights
  m_LightRegistry.update();

  // RenderPass setup stage
  m_passManager.setup();

  // RenderPass render/cleanup stages
  m_passManager.render();
  m_passManager.cleanup();

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
    size_t subMeshId = id;
    subMeshId <<= 32;
    subMeshId |= i;

    m_meshRegistry.submit(subMeshId, std::move(info));

    ++i;
  }
}

void Renderer::registerPointLight(const glm::vec3 &position, const PointLightComponent &pointLight, size_t identifier)
{
  m_LightRegistry.submitLight(pointLight, position, identifier);
}

void Renderer::unregisterPointLight(size_t identifier)
{
  m_LightRegistry.removePointLight(identifier);
}

void Renderer::updatePointLight(const glm::vec3& position, const PointLightComponent& pointLight, size_t identifier)
{
  m_LightRegistry.updateLight(pointLight, position, identifier);
}

void Renderer::registerDirectionalLight(const DirectionalLightComponent& dirLight, const glm::vec3& direction, size_t identifier)
{
  m_LightRegistry.submitLight(dirLight, direction, identifier);
}

void Renderer::unregisterDirectionalLight(size_t identifier)
{
  m_LightRegistry.removeDirectionalLight(identifier);
}

void Renderer::updateDirectionalLight(const DirectionalLightComponent& dirLight, const glm::vec3& direction, size_t identifier)
{
  m_LightRegistry.updateLight(dirLight, direction, identifier);
}

const glm::uvec2 &Renderer::getViewportOrigin() const
{
  return m_ViewportOrigin;
}

const glm::uvec2 &Renderer::getViewportSize() const
{
  return m_ViewportSize;
}

void Renderer::setViewport(const glm::uvec2 &o, const glm::uvec2 &s)
{
  setViewportOrigin(o);
  setViewportSize(s);
}

void Renderer::setViewportOrigin(const glm::uvec2 &o)
{
  m_ViewportOrigin = o;
}

void Renderer::setViewportSize(const glm::uvec2 &s)
{
  m_ViewportSize = s;
  m_passManagerDirty = true;
}
