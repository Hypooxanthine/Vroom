#pragma once

#include "Vroom/Api.h"
#include "Vroom/Render/ParticleEmitterRegistry.h"
#include "Vroom/Render/Passes/RenderPassManager.h"
#include "Vroom/Render/RenderResources.h"
#include "Vroom/Render/DynamicRenderSettings.h"
#include "Vroom/Render/RenderSettings.h"

namespace vrm
{

  class MeshRegistry;
  class LightRegistry;
  class RenderSkybox;

  // This should definitely become a RenderScene
  // It will probably be needed when the engine will support culling.
  struct RenderPipelineContext
  {
    MeshRegistry* meshes = nullptr;
    LightRegistry* lights = nullptr;
    RenderSkybox* skybox = nullptr;
    ParticleEmitterRegistry* particleEmitters = nullptr;
  };

  class VRM_API RenderPipeline
  {
  public:

    friend struct RendererAttorney;

  public:

    RenderPipeline();
    ~RenderPipeline();
    
    RenderPipeline& operator=(const RenderPipeline& other) = delete;
    RenderPipeline(const RenderPipeline& other) = delete;
    
    RenderPipeline& operator=(RenderPipeline&& other) = default;
    RenderPipeline(RenderPipeline&& other) = default;

    inline void setContext(const RenderPipelineContext& context) { m_context = context; }
    
    void generateIfDirty();
    void generate();
    void execute(RenderPassContext& context);

    void setRenderSettings(const RenderSettings& settings);
    inline void setDynamicRenderSettings(const DynamicRenderSettings& settings) { m_dynamicSettings = settings; }

    inline const RenderSettings& getRenderSettings() const { return m_renderSettings; }
    inline const DynamicRenderSettings& getDynamicRenderSettings() const { return m_dynamicSettings; }

    const gl::Texture* getRenderedTexture() const { return m_finalTexture; }
    
    const std::vector<std::string>& getExposedTextureNames() const { return m_resources.getExposedTextures(); }
    void watchExposedTexture(const std::string& name);
    const std::string& getWatchedTexture() const { return m_watchedTexture; }

    uint32_t getEntityIndexOnPixel(const glm::ivec2& px) const;

  public:

    struct RendererAttorney
    {
      friend RenderPipeline;
      friend class Renderer;

    private:
      RendererAttorney(RenderPipeline& pipeline) : pipeline(pipeline) {}

      RenderPipeline& pipeline;

      inline void setFrameSize(const glm::uvec2& frameSize) { pipeline.m_renderSettings.frameSize = frameSize; pipeline.m_dirty = true; }
    };

    inline RendererAttorney getRendererAttorney() { return RendererAttorney(*this); }

  private:

    void _updateFinalTextureWithWatched();

  private:

    RenderPassManager m_passManager;    
    
    RenderPipelineContext m_context;
    RenderSettings m_renderSettings;
    DynamicRenderSettings m_dynamicSettings;
    
    RenderResources m_resources;
    gl::Texture* m_finalTexture = nullptr;
    gl::FrameBuffer* m_renderFrameBuffer = nullptr;

    // Optional: m_finalTexture will point to the watched texture, if asked
    // --> useful to visualize an intermediate texture resource of the pipeline
    std::string m_watchedTexture = "";

    bool m_dirty = true;

  };

}