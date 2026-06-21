#include "Renderer/RenderPipeline.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "Rasterizer/Buffer.h"
#include "Rasterizer/GLCore.h"
#include "Renderer/BlitFrameBufferPass.h"
#include "Renderer/ClearFrameBufferPass.h"
#include "Renderer/DrawScenePass.h"
#include "Renderer/GaussianBlurPass.h"
#include "Renderer/LightClusteringPass.h"
#include "Renderer/RenderParticlesPass.h"
#include "Renderer/RenderPassFactory.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/RenderSkyboxPass.h"
#include "Renderer/ShadowMappingPass.h"
#include "Renderer/ToneMappingPass.h"

using namespace vrm;

struct RenderPipeline::PickingState
{
  struct Request
  {
    glm::ivec2       pixel = { 0, 0 };
    Future<uint32_t> future;
    gl::Buffer       pbo;
    GLsync           fence  = nullptr;
    bool             issued = false;
  };

  std::vector<Request> requests;
};

RenderPipeline::RenderPipeline()
  : m_picking(std::make_unique<PickingState>())
{}

RenderPipeline::~RenderPipeline()
{
  // GLsync handles outlive RenderResources, release them explicitly.
  for (auto& req : m_picking->requests)
    if (req.fence != nullptr)
      glDeleteSync(req.fence);
}

void RenderPipeline::generateIfDirty()
{
  if (m_dirty)
  {
    generate();
    m_dirty = false;
  }
}

void RenderPipeline::generate()
{
  // Ensure the GPU has finished with any in-flight resources before we delete them.
  // Without this, the driver inserts implicit sync points over subsequent frames,
  // causing stuttering / long SwapBuffers times.
  glFinish();

  m_passManager.reset();
  m_resources.clear();
  m_finalTexture      = nullptr;
  m_renderFrameBuffer = nullptr;

  gl::Texture* hdrFlatTexture               = nullptr;
  gl::Texture* flatZBuffer                  = nullptr;
  gl::Texture* bloomBrightnessTextureNoMsaa = nullptr;

  auto aa   = m_renderSettings.antiAliasingLevel;
  bool aaOK = (aa != 0 && ((aa & (aa - 1)) == 0));
  VRM_ASSERT_MSG(aaOK, "Invalid antialiasing value: {}", aa);

  // First custom passes
  _pushCustomPasses(ECustomSlot::eFirst);

  // Render frame buffer
  {
    auto& fb = *m_resources.genFramebuffer("RenderFramebuffer");
    fb.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, aa);

    gl::Texture::Desc texDesc;
    {
      texDesc.dimension   = 2;
      texDesc.width       = m_renderSettings.frameSize.x;
      texDesc.height      = m_renderSettings.frameSize.y;
      texDesc.sampleCount = aa;
    }

    auto& colorTex = *m_resources.genTexture("RenderColorBuffer", texDesc.sampleCount == 1);
    {
      texDesc.format         = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F; // HDR
      colorTex.create(texDesc);
      fb.setColorAttachment(0, colorTex);
    }

    if (m_renderSettings.bloom.activated)
    {
      gl::Texture* brightnessTex = m_resources.genTexture("BrightnessColorBuffer", texDesc.sampleCount == 1);
      texDesc.format             = GL_RGBA;
      texDesc.internalFormat     = GL_RGBA16F;
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
      texDesc.format         = GL_DEPTH_COMPONENT;
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
      flatZBuffer    = &depthTex;
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
    // The shadow map is resolution-independent, so it lives outside the
    // per-rebuild RenderResources and persists across rebuilds. Thanks to the
    // idempotent gl::Texture::create(), its (large) depth array is not
    // reallocated when the pipeline is rebuilt (e.g. on a viewport resize).
    auto& pass = m_passManager.pushPass<ShadowMappingPass>();

    pass.lights              = m_context.lights;
    pass.meshRegistry        = m_context.meshes;
    pass.resolution          = 4096;
    pass.depthTextureArray   = &m_dirLightShadowMaps;
    pass.lightMatricesBuffer = m_resources.genAutoBuffer("LightMatricesStorageBuffer");
  }
  else
  {
    // Free the persistent shadow map when shadows are disabled.
    m_dirLightShadowMaps.release();
  }

  LightClusteringPass* lightClusteringPass = nullptr;

  // Light clustering
  if (m_renderSettings.clusteredShading)
  {
    auto& pass               = m_passManager.pushPass<LightClusteringPass>();
    pass.clusterCount        = m_renderSettings.clusterCount;
    pass.lightsStorageBuffer = &m_context.lights->getPointLightsStorageBuffer();

    lightClusteringPass = &pass;
  }

  // Main scene rendering
  {
    auto& pass = m_passManager.pushPass<DrawSceneRenderPass>();
    pass.meshTags.set(EMeshTag::eVisible);
    pass.meshRegistry      = m_context.meshes;
    pass.framebufferTarget = m_renderFrameBuffer;
    pass.faceCulling       = DrawSceneRenderPass::EFaceCulling::eCullBack;
    pass.frontFace         = DrawSceneRenderPass::EFrontFace::eCCW;
    pass.storageBufferParameters.emplace_back("PointLightBlock", &m_context.lights->getPointLightsStorageBuffer());
    pass.storageBufferParameters.emplace_back("DirLightBlock", &m_context.lights->getDirLightsStorageBuffer());
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
      pass.clusteredLightsBuffer     = &lightClusteringPass->getClustersBuffer();
      pass.clusteredLightPerViewSize = lightClusteringPass->getPerViewSize();
    }

    if (m_renderSettings.showLightComplexity)
      pass.addDefine("VRM_LIGHT_COMPLEXITY");

    pass.shadowsEnable = m_renderSettings.shadowsEnable;
    if (m_renderSettings.shadowsEnable)
    {
      pass.addDefine("VRM_DIR_LIGHTS_SHADOWS");
      pass.dirLightShadowMaps = &m_dirLightShadowMaps;
      pass.storageBufferParameters.emplace_back(
        "LightMatricesBlock", &m_resources.tryGetAutoBuffer("LightMatricesStorageBuffer")->getBuffer());
    }
  }

  _pushCustomPasses(ECustomSlot::ePostRenderScene);

  // Skybox
  {
    auto& pass = m_passManager.pushPass<RenderSkyboxPass>();

    pass.skybox      = m_context.skybox;
    pass.framebuffer = m_renderFrameBuffer;
  }

  // Particles
  {
    auto& pass    = m_passManager.pushPass<RenderParticlesPass>();
    pass.emitters = m_context.particleEmitters;
  }

  // Entity picking
  if (m_entityPickingEnabled)
  {
    auto&             tex = *m_resources.genTexture("PickingColorTexture");
    gl::Texture::Desc desc;
    {
      desc.dimension      = 2;
      desc.width          = m_renderSettings.frameSize.x;
      desc.height         = m_renderSettings.frameSize.y;
      desc.internalFormat = GL_R32UI;
      desc.format         = GL_RED_INTEGER;
    }
    tex.create(desc);

    auto& depth = *m_resources.genTexture("PickingDepthTexture");
    {
      desc.internalFormat = GL_DEPTH_COMPONENT24;
      desc.format         = GL_DEPTH_COMPONENT;
    }
    depth.create(desc);

    auto& fb = *m_resources.genFramebuffer("PickingFrameBuffer");
    fb.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, 1);
    fb.setColorAttachment(0, tex);
    fb.setDepthAttachment(depth);

    VRM_ASSERT_MSG(fb.validate(), "Could not build picking framebuffer");

    {
      auto& pass       = m_passManager.pushPass<ClearFrameBufferPass>();
      pass.framebuffer = &fb;
    }

    {
      auto& pass = m_passManager.pushPass<DrawSceneRenderPass>();
      pass.addDefine("VRM_ENTITY_PICKING");
      pass.meshTags.set(EMeshTag::eVisible);
      pass.meshRegistry      = m_context.meshes;
      pass.framebufferTarget = &fb;
      pass.faceCulling       = DrawSceneRenderPass::EFaceCulling::eCullBack;
      pass.frontFace         = DrawSceneRenderPass::EFrontFace::eCCW;
      pass.shadowsEnable     = false;
      pass.bloomEnable       = false;
    }
  }

  // MSAA
  if (aa > 1)
  {
    // We must resolve AA by blitting the render framebuffer
    // into another one
    auto& resolvedFb = *m_resources.genFramebuffer("MsaaResolvedFramebuffer");
    resolvedFb.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, 1);

    gl::Texture::Desc texDesc;
    {
      texDesc.dimension   = 2;
      texDesc.width       = m_renderSettings.frameSize.x;
      texDesc.height      = m_renderSettings.frameSize.y;
      texDesc.sampleCount = 1;
    }

    hdrFlatTexture = m_resources.genTexture("MsaaResolvedColorBuffer", true);
    {
      texDesc.format         = GL_RGBA;
      texDesc.internalFormat = GL_RGBA16F;
      hdrFlatTexture->create(texDesc);
      resolvedFb.setColorAttachment(0, *hdrFlatTexture);
    }

    if (m_renderSettings.bloom.activated)
    {
      // Overriding the brightness texture
      bloomBrightnessTextureNoMsaa = m_resources.genTexture("NoMsaaBrightnessColorBuffer", true);
      texDesc.format               = GL_RGBA;
      texDesc.internalFormat       = GL_RGBA16F;
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
      texDesc.format         = GL_DEPTH_COMPONENT;
      texDesc.internalFormat = GL_DEPTH_COMPONENT24;
      flatZBuffer->create(texDesc);
      GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
      glTexParameteriv(flatZBuffer->getDefaultTarget(), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
      resolvedFb.setDepthAttachment(*flatZBuffer);
    }

    VRM_ASSERT_MSG(resolvedFb.validate(), "Could not build MSAA resolved framebuffer");

    auto& pass       = m_passManager.pushPass<BlitFrameBufferPass>();
    pass.source      = m_renderFrameBuffer;
    pass.destination = &resolvedFb;
  }

  // Gaussian blur
  if (m_renderSettings.bloom.activated)
  {
    gl::Texture::Desc texDesc;
    {
      texDesc.dimension      = 2;
      texDesc.width          = m_renderSettings.frameSize.x;
      texDesc.height         = m_renderSettings.frameSize.y;
      texDesc.sampleCount    = 1;
      texDesc.format         = GL_RGBA;
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
    framebufferA.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, 1);
    framebufferA.setColorAttachment(0, interColorBuffer);
    VRM_ASSERT_MSG(framebufferA.validate(), "Could not validate GaussianBlurframebufferA");

    gl::FrameBuffer& framebufferB = *m_resources.genFramebuffer("GaussianBlurFramebufferB");
    framebufferB.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, 1);
    framebufferB.setColorAttachment(0, *bloomBrightnessTextureNoMsaa);
    VRM_ASSERT_MSG(framebufferB.validate(), "Could not validate GaussianBlurFramebufferB");

    GaussianBlurPass& pass   = m_passManager.pushPass<GaussianBlurPass>();
    pass.framebufferA        = &framebufferA;
    pass.framebufferB        = &framebufferB;
    pass.texture             = bloomBrightnessTextureNoMsaa; // Will store the final blurred texture.
    pass.intermediateTexture = &interColorBuffer;
  }

  gl::Texture* rgbFlatTexture = nullptr;

  // Tone mapping
  {
    gl::Texture::Desc texDesc;
    {
      texDesc.dimension      = 2;
      texDesc.width          = m_renderSettings.frameSize.x;
      texDesc.height         = m_renderSettings.frameSize.y;
      texDesc.sampleCount    = 1;
      texDesc.format         = GL_RGBA;
      texDesc.internalFormat = GL_SRGB8_ALPHA8;
    }

    rgbFlatTexture = m_resources.genTexture("rgbFlatTexture");
    rgbFlatTexture->create(texDesc);

    gl::FrameBuffer& fb = *m_resources.genFramebuffer("hdrResolveFrameBuffer");
    fb.create(m_renderSettings.frameSize.x, m_renderSettings.frameSize.y, 1);
    fb.setColorAttachment(0, *rgbFlatTexture);
    VRM_ASSERT_MSG(fb.validate(), "Could not build hdrResolveFrameBuffer");

    ToneMappingPass& pass = m_passManager.pushPass<ToneMappingPass>();

    pass.hdrTex            = hdrFlatTexture;
    pass.framebufferTarget = &fb;

    if (m_renderSettings.bloom.activated)
    {
      pass.blurredBrightness = bloomBrightnessTextureNoMsaa;
      pass.addDefine("VRM_BLOOM_ENABLE");
    }
  }

  _pushCustomPasses(ECustomSlot::eEnd);

  _updateFinalTextureWithWatched();

  m_passManager.init();
}

void RenderPipeline::execute(RenderPassContext& context)
{
  context.dynamicSettings   = &m_dynamicSettings;
  context.frameBufferTarget = m_renderFrameBuffer;

  // RenderPass setup stage
  m_passManager.setup(context);

  // RenderPass render/cleanup stages
  m_passManager.render(context);
  m_passManager.cleanup(context);

  // The picking framebuffer now holds this frame's entity ids: issue any
  // queued readbacks and resolve the ones the GPU has finished with.
  _processEntityPicking();
}

void RenderPipeline::setEntityPickingEnabled(bool enabled)
{
  if (m_entityPickingEnabled != enabled)
  {
    m_entityPickingEnabled = enabled;
    m_dirty                = true;
  }
}

uint32_t RenderPipeline::getEntityIndexOnPixel(const glm::ivec2& px) const
{
  if (!m_entityPickingEnabled)
    return 0;

  const gl::FrameBuffer* fb = m_resources.tryGetFramebuffer("PickingFrameBuffer");

  if (fb == nullptr)
    return 0;

  fb->bind();
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  uint32_t pixel;
  glReadPixels(px.x, px.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
  glReadBuffer(GL_NONE);
  return pixel;
}

Promise<uint32_t> RenderPipeline::pickEntityIndex(const glm::ivec2& px)
{
  if (!m_entityPickingEnabled)
  {
    Future<uint32_t>  future;
    Promise<uint32_t> promise = future.createPromise();
    future.setValue(0u);
    return promise;
  }

  PickingState::Request request;
  request.pixel             = px;
  Promise<uint32_t> promise = request.future.createPromise();

  m_picking->requests.push_back(std::move(request));

  return promise;
}

void RenderPipeline::_processEntityPicking()
{
  if (m_picking->requests.empty())
    return;

  const gl::FrameBuffer* fb = m_resources.tryGetFramebuffer("PickingFrameBuffer");

  // Pipeline not built yet (or picking disabled): leave the requests queued,
  // they will be issued once the picking framebuffer exists.
  if (fb == nullptr)
    return;

  // 1: Issue a non-blocking readback for every request that doesn't have one yet.
  for (auto& request : m_picking->requests)
  {
    if (request.issued)
      continue;

    gl::Buffer::Desc desc;
    desc.capacity        = sizeof(uint32_t);
    desc.allowMapReading = true;
    request.pbo.create(desc);

    fb->bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    gl::Buffer::Bind(request.pbo, GL_PIXEL_PACK_BUFFER);
    // Reading into the bound PBO: the last argument is a byte offset.
    glReadPixels(request.pixel.x, request.pixel.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    gl::Buffer::Unbind(GL_PIXEL_PACK_BUFFER);
    glReadBuffer(GL_NONE);

    request.fence  = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    request.issued = true;
  }

  // 2: Resolve finished requests
  std::vector<std::pair<Future<uint32_t>, uint32_t>> ready;

  for (auto it = m_picking->requests.begin(); it != m_picking->requests.end();)
  {
    if (it->fence == nullptr)
    {
      ++it;
      continue;
    }

    const GLenum status = glClientWaitSync(it->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
    if (status != GL_ALREADY_SIGNALED && status != GL_CONDITION_SATISFIED)
    {
      ++it; // GL_TIMEOUT_EXPIRED: not ready this frame.
      continue;
    }

    uint32_t pixel = 0;
    gl::Buffer::Bind(it->pbo, GL_PIXEL_PACK_BUFFER);
    if (const void* mapped = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, sizeof(uint32_t), GL_MAP_READ_BIT))
    {
      std::memcpy(&pixel, mapped, sizeof(uint32_t));
      glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }
    gl::Buffer::Unbind(GL_PIXEL_PACK_BUFFER);

    glDeleteSync(it->fence);

    ready.emplace_back(std::move(it->future), pixel);
    it = m_picking->requests.erase(it);
  }

  for (auto& [future, pixel] : ready)
    future.setValue(pixel);
}

void RenderPipeline::addCustomPass(ECustomSlot slot, std::unique_ptr<RenderPassFactory>&& passFactory)
{
  m_customPasses.at((size_t)slot).pushPass(std::move(passFactory));
  m_dirty = true;
}

void RenderPipeline::setRenderSettings(const RenderSettings& settings)
{
  if (settings != m_renderSettings)
  {
    m_renderSettings = settings;
    m_dirty          = true;
  }
}

void RenderPipeline::setFrameSize(const glm::uvec2& frameSize)
{
  RenderSettings settings = m_renderSettings;
  settings.frameSize      = frameSize;
  setRenderSettings(settings);
}

glm::uvec2 RenderPipeline::getFrameSize() const
{
  return m_renderSettings.frameSize;
}

void RenderPipeline::watchExposedTexture(const std::string& name)
{
  if (name == m_watchedTexture)
    return;

  m_watchedTexture = name;

  _updateFinalTextureWithWatched();
}

void RenderPipeline::_pushCustomPasses(ECustomSlot slot)
{
  for (const std::unique_ptr<RenderPassFactory>& factory : m_customPasses.at((size_t)slot))
  {
    m_passManager.pushPass(factory->createPass());
  }
}

void RenderPipeline::_updateFinalTextureWithWatched()
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
