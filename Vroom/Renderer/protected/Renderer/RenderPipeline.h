#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include "RenderObjects/fwds.h"
#include "Renderer/Api.h"
#include "Renderer/CustomPassSlot.h"
#include "Renderer/DynamicRenderSettings.h"
#include "Renderer/ParticleEmitterRegistry.h"
#include "Renderer/RenderPassFactory.h"
#include "Renderer/RenderPassManager.h"
#include "Renderer/RenderResources.h"
#include "Renderer/RenderSettings.h"
#include "Tools/Promise.hpp"

namespace vrm
{

class MeshRegistry;
class LightRegistry;

// This should definitely become a RenderScene
// It will probably be needed when the engine will support culling.
struct RenderPipelineContext
{
  MeshRegistry*            meshes           = nullptr;
  LightRegistry*           lights           = nullptr;
  const render::Cubemap**  skybox           = nullptr;
  ParticleEmitterRegistry* particleEmitters = nullptr;
};

class VRM_RENDERER_API RenderPipeline
{
public:

  friend struct RendererAttorney;

  enum class ECustomSlot
  {
    eFirst = 0,
    ePostRenderScene,
    eEnd,
    eCount
  };

public:

  RenderPipeline();
  ~RenderPipeline();

  RenderPipeline& operator=(const RenderPipeline& other) = delete;
  RenderPipeline(const RenderPipeline& other)            = delete;

  RenderPipeline& operator=(RenderPipeline&& other) = delete;
  RenderPipeline(RenderPipeline&& other)            = delete;

  inline void setContext(const RenderPipelineContext& context)
  {
    m_context = context;
  }

  inline const RenderPipelineContext& getContext() const
  {
    return m_context;
  }

  void generateIfDirty();
  void generate();
  void execute(RenderPassContext& context);

  void addCustomPass(ECustomSlot slot, std::unique_ptr<RenderPassFactory>&& passFactory);

  void        setRenderSettings(const RenderSettings& settings);
  inline void setDynamicRenderSettings(const DynamicRenderSettings& settings)
  {
    m_dynamicSettings = settings;
  }

  void setFrameSize(const glm::uvec2& frameSize);
  glm::uvec2 getFrameSize() const;

  inline const RenderSettings& getRenderSettings() const
  {
    return m_renderSettings;
  }
  inline const DynamicRenderSettings& getDynamicRenderSettings() const
  {
    return m_dynamicSettings;
  }

  const gl::Texture* getRenderedTexture() const
  {
    return m_finalTexture;
  }

  const std::vector<std::string>& getExposedTextureNames() const
  {
    return m_resources.getExposedTextures();
  }
  void               watchExposedTexture(const std::string& name);
  const std::string& getWatchedTexture() const
  {
    return m_watchedTexture;
  }

  void setEntityPickingEnabled(bool enabled = true);

  /**
   * @brief Synchronously reads back the entity index under a pixel.
   *
   * Issues a blocking glReadPixels: it stalls the GPU pipeline. Prefer
   * pickEntityIndex() unless the result is needed within the same call.
   */
  uint32_t getEntityIndexOnPixel(const glm::ivec2& px) const;

  /**
   * @brief Asynchronously reads back the entity index under a pixel, not stalling the GPU.
   */
  Promise<uint32_t> pickEntityIndex(const glm::ivec2& px);

private:

  void _pushCustomPasses(ECustomSlot slot);
  void _updateFinalTextureWithWatched();

  // Issues pending asynchronous picking readbacks and resolves the ones whose
  // GPU readback has completed. Called once per frame from execute().
  void _processEntityPicking();

private:

  std::array<CustomPassSlot, (size_t)ECustomSlot::eCount> m_customPasses;

  RenderPassManager m_passManager;

  RenderPipelineContext m_context;
  RenderSettings        m_renderSettings;
  DynamicRenderSettings m_dynamicSettings;

  RenderResources m_resources;

  // Resolution-independent (always 4096^2), kept here rather than in the
  // per-rebuild RenderResources so its large depth array survives pipeline
  // rebuilds instead of being reallocated on every resize. See generate().
  gl::Texture m_dirLightShadowMaps;

  gl::Texture*     m_finalTexture      = nullptr;
  gl::FrameBuffer* m_renderFrameBuffer = nullptr;

  // Optional: m_finalTexture will point to the watched texture, if asked
  // --> useful to visualize an intermediate texture resource of the pipeline
  std::string m_watchedTexture = "";

  bool m_entityPickingEnabled = true;

  // Holds the in-flight asynchronous picking readbacks (PBO + fence + future).
  struct PickingState;
  std::unique_ptr<PickingState> m_picking;

  bool m_dirty = true;
};

} // namespace vrm
