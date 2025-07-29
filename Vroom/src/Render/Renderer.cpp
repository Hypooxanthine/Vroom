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
#include "Vroom/Render/Passes/ToneMappingPass.h"
#include "Vroom/Render/Passes/GaussianBlurPass.h"

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/Shader.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/Camera/CameraBasic.h"

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

void Renderer::setDynamicRenderSettings(const DynamicRenderSettings& settings)
{
  m_dynamicSettings = settings;
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
  m_resources.clear();
  m_finalTexture = nullptr;
  m_renderFrameBuffer = nullptr;

  gl::Texture* hdrFlatTexture = nullptr;
  gl::Texture* flatZBuffer = nullptr;
  gl::Texture* bloomBrightnessTextureNoMsaa = nullptr;

  auto aa = m_renderSettings.antiAliasingLevel;
  bool aaOK = (aa != 0 && ((aa & (aa - 1)) == 0));
  VRM_ASSERT_MSG(aaOK, "Invalid antialiasing value: {}", aa);

  // Render frame buffer
  {
    auto& fb = *m_resources.genFramebuffer("RenderFramebuffer");
    fb.create(m_viewport.getSize().x, m_viewport.getSize().y, aa);

    gl::Texture::Desc texDesc;
    {
      texDesc.dimension = 2;
      texDesc.width = m_viewport.getSize().x;
      texDesc.height = m_viewport.getSize().y;
      texDesc.sampleCount = aa;
    }

    auto& colorTex = *m_resources.genTexture("RenderColorBuffer", texDesc.sampleCount == 1);
    {
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F; // HDR
      colorTex.create(texDesc);
      fb.setColorAttachment(0, colorTex);
    }

    if (m_renderSettings.bloom.activated)
    {
      gl::Texture* brightnessTex = m_resources.genTexture("BrightnessColorBuffer", texDesc.sampleCount == 1);
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F;
      brightnessTex->create(texDesc);
      brightnessTex->bind();
      glTexParameteri(brightnessTex->getDefaultTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(brightnessTex->getDefaultTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(brightnessTex->getDefaultTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(brightnessTex->getDefaultTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      fb.setColorAttachment(1, *brightnessTex);

      if (aa == 1)
      {
        bloomBrightnessTextureNoMsaa = brightnessTex;
      }
      // else -> will be set in the msaa resolving step
    }

    auto& depthTex = *m_resources.genTexture("RenderDepthBuffer", texDesc.sampleCount == 1);
    {
      texDesc.format = GL_DEPTH_COMPONENT;
      texDesc.internalFormat = GL_DEPTH_COMPONENT24;
      depthTex.create(texDesc);
      GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
      glTexParameteriv(depthTex.getDefaultTarget(), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
      fb.setDepthAttachment(depthTex);
    }

    VRM_ASSERT_MSG(fb.validate(), "Could not build render framebuffer");

    m_renderFrameBuffer = &fb;
    if (aa == 1)
    {
      hdrFlatTexture = &colorTex;
      flatZBuffer = &depthTex;
    }
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
    auto& maps = *m_resources.genTexture("DirLightsShadowMaps");

    auto& pass = m_passManager.pushPass<ShadowMappingPass>();

    pass.lights = &m_LightRegistry;
    pass.meshRegistry = &m_meshRegistry;
    pass.resolution = 4096;
    pass.depthTextureArray = &maps;
    pass.lightMatricesBuffer = m_resources.genAutoBuffer("LightMatricesStorageBuffer");
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
    pass.addDefine("VRM_MAIN_RENDER_PASS");

    if (m_renderSettings.normalMapping.activated)
    {
      pass.addDefine("VRM_NORMAL_MAPPING");
      if (m_renderSettings.normalMapping.reorthoTangentSpace)
      {
        pass.addDefine("VRM_REORTHOGONALIZE_TANGENT_SPACE");
      }
    }

    if (m_renderSettings.bloom.activated)
    {
      pass.bloomEnable = true;
      pass.addDefine("VRM_EXTRACT_BRIGHTNESS");
      pass.addDefine("VRM_BRIGHTNESS_OUT_SLOT", 1);
    }

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
      pass.dirLightShadowMaps = m_resources.tryGetTexture("DirLightsShadowMaps");
      pass.storageBufferParameters["LightMatricesBlock"] = &m_resources.tryGetAutoBuffer("LightMatricesStorageBuffer")->getBuffer();
    }
  }

  // Entity picking
  {
    auto& tex = *m_resources.genTexture("PickingColorTexture");
    gl::Texture::Desc desc;
    {
      desc.dimension = 2;
      desc.width = m_viewport.getSize().x;
      desc.height = m_viewport.getSize().y;
      desc.internalFormat = GL_R32UI;
      desc.format = GL_UNSIGNED_INT;
    }
    tex.create(desc);
    
    auto& depth = *m_resources.genTexture("PickingDepthTexture");
    {
      desc.internalFormat = GL_DEPTH_COMPONENT24;
      desc.format = GL_DEPTH_COMPONENT;
    }
    depth.create(desc);
    
    auto& fb = *m_resources.genFramebuffer("PickingFrameBuffer");
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
    // We must resolve AA by blitting the render framebuffer
    // into another one
    auto& resolvedFb = *m_resources.genFramebuffer("MsaaResolvedFramebuffer");
    resolvedFb.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);

    gl::Texture::Desc texDesc;
    {
      texDesc.dimension = 2;
      texDesc.width = m_viewport.getSize().x;
      texDesc.height = m_viewport.getSize().y;
      texDesc.sampleCount = 1;
    }

    hdrFlatTexture = m_resources.genTexture("MsaaResolvedColorBuffer", true);
    {
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F;
      hdrFlatTexture->create(texDesc);
      resolvedFb.setColorAttachment(0, *hdrFlatTexture);
    }

    if (m_renderSettings.bloom.activated)
    {
      // Overriding the brightness texture
      bloomBrightnessTextureNoMsaa = m_resources.genTexture("NoMsaaBrightnessColorBuffer", true);
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F;
      bloomBrightnessTextureNoMsaa->create(texDesc);
      bloomBrightnessTextureNoMsaa->bind();
      glTexParameteri(bloomBrightnessTextureNoMsaa->getDefaultTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(bloomBrightnessTextureNoMsaa->getDefaultTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(bloomBrightnessTextureNoMsaa->getDefaultTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(bloomBrightnessTextureNoMsaa->getDefaultTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      resolvedFb.setColorAttachment(1, *bloomBrightnessTextureNoMsaa);
    }

    flatZBuffer = m_resources.genTexture("MsaaResolvedDepthBuffer", true);
    {
      texDesc.format = GL_DEPTH_COMPONENT;
      texDesc.internalFormat = GL_DEPTH_COMPONENT24;
      flatZBuffer->create(texDesc);
      GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
      glTexParameteriv(flatZBuffer->getDefaultTarget(), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
      resolvedFb.setDepthAttachment(*flatZBuffer);
    }

    VRM_ASSERT_MSG(resolvedFb.validate(), "Could not build MSAA resolved framebuffer");

    auto& pass = m_passManager.pushPass<BlitFrameBufferPass>();
    pass.source = m_renderFrameBuffer;
    pass.destination = &resolvedFb;
  }

  // Gaussian blur
  if (m_renderSettings.bloom.activated)
  {
    gl::Texture::Desc texDesc;
    {
      texDesc.dimension = 2;
      texDesc.width = m_viewport.getSize().x;
      texDesc.height = m_viewport.getSize().y;
      texDesc.sampleCount = 1;
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F;
    }

    gl::Texture& interColorBuffer = *m_resources.genTexture("GaussianBlurIntermediateColorBuffer");
    interColorBuffer.create(texDesc);
    interColorBuffer.bind();
    glTexParameteri(interColorBuffer.getDefaultTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(interColorBuffer.getDefaultTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(interColorBuffer.getDefaultTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(interColorBuffer.getDefaultTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl::FrameBuffer& framebufferA = *m_resources.genFramebuffer("GaussianBlurFramebufferA");
    framebufferA.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
    framebufferA.setColorAttachment(0, interColorBuffer);
    VRM_ASSERT_MSG(framebufferA.validate(), "Could not validate GaussianBlurframebufferA");

    gl::FrameBuffer& framebufferB = *m_resources.genFramebuffer("GaussianBlurFramebufferB");
    framebufferB.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
    framebufferB.setColorAttachment(0, *bloomBrightnessTextureNoMsaa);
    VRM_ASSERT_MSG(framebufferB.validate(), "Could not validate GaussianBlurFramebufferB");

    GaussianBlurPass& pass = m_passManager.pushPass<GaussianBlurPass>();
    pass.framebufferA = &framebufferA;
    pass.framebufferB = &framebufferB;
    pass.texture = bloomBrightnessTextureNoMsaa; // Will store the final blurred texture.
    pass.intermediateTexture = &interColorBuffer;
  }

  gl::Texture* rgbFlatTexture = nullptr;

  // Tone mapping
  {
    gl::Texture::Desc texDesc;
    {
      texDesc.dimension = 2;
      texDesc.width = m_viewport.getSize().x;
      texDesc.height = m_viewport.getSize().y;
      texDesc.sampleCount = 1;
      texDesc.format = GL_RGBA;
      texDesc.internalFormat = GL_SRGB8_ALPHA8;
    }

    rgbFlatTexture = m_resources.genTexture("rgbFlatTexture");
    rgbFlatTexture->create(texDesc);

    gl::FrameBuffer& fb = *m_resources.genFramebuffer("hdrResolveFrameBuffer");
    fb.create(m_viewport.getSize().x, m_viewport.getSize().y, 1);
    fb.setColorAttachment(0, *rgbFlatTexture);
    VRM_ASSERT_MSG(fb.validate(), "Could not build hdrResolveFrameBuffer");

    ToneMappingPass& pass = m_passManager.pushPass<ToneMappingPass>();

    pass.hdrTex = hdrFlatTexture;
    pass.framebufferTarget = &fb;

    if (m_renderSettings.bloom.activated)
    {
      pass.blurredBrightness = bloomBrightnessTextureNoMsaa;
      pass.addDefine("VRM_BLOOM_ENABLE");
    }
  }

  updateFinalTextureWithWatched();

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
  renderContext.dynamicSettings = &m_dynamicSettings;
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

void Renderer::watchExposedTexture(const std::string& name)
{
  if (name == m_watchedTexture)
    return;

  m_watchedTexture = name;

  updateFinalTextureWithWatched();
}

void Renderer::updateFinalTextureWithWatched()
{
  if (m_watchedTexture != "")
  {
    if (m_resources.isTextureExposed(m_watchedTexture))
    {
      m_finalTexture = m_resources.tryGetTexture(m_watchedTexture);
      if (m_finalTexture)
      {
        return; // OK
      }
    }
    else
    {
      m_watchedTexture = "";
    }
  }

  // Fallback : real color buffer
  m_finalTexture = m_resources.tryGetTexture("rgbFlatTexture");
}

uint32_t Renderer::getEntityIndexOnPixel(const glm::ivec2& px) const
{
  const gl::FrameBuffer* fb = m_resources.tryGetFramebuffer("PickingFrameBuffer");
  if (fb == nullptr)
  {
    return 0;
  }
  
  fb->bind();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  uint32_t pixel;
  glReadPixels(px.x, px.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
  glReadBuffer(GL_NONE);
  return pixel;
}
