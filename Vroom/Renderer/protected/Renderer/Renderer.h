#pragma once

#include <glm/glm.hpp>

#include "RenderObjects/Cubemap.h"
#include "RenderObjects/fwds.h"
#include "Renderer/Api.h"
#include "Renderer/GPURuntimeFeatures.h"
#include "Renderer/LightRegistry.h"
#include "Renderer/MeshRegistry.h"
#include "Renderer/ParticleEmitterRegistry.h"
#include "Renderer/RenderPipeline.h"

namespace vrm
{

class RenderLayout;

namespace gl
{
class FrameBuffer;
}

/**
 * @brief The renderer is responsible for rendering objects on the scene, taking lights and cameras into consideration.
 *
 */
class VRM_RENDERER_API Renderer
{
public:

  Renderer();
  Renderer(const Renderer&)            = delete;
  Renderer(Renderer&&)                 = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&)      = delete;

  /**
   * @brief Releases GPU memory.
   */
  ~Renderer();

  const GPURuntimeFeatures& getGPUFeatures() const;

  /**
   * @brief Has to be called before any rendering of current frame.
   *
   */
  void beginScene(const RenderLayout* layout);

  /**
   * @brief Has to be called after any rendering scene.
   *
   */
  void endScene();

  void submitMesh(const MeshRenderInfo& meshInfo);

  void submitSkybox(const render::Cubemap& skybox);

  void submitPointLight(size_t id, const render::PointLight& pointLight);

  void submitDirectionalLight(size_t id, const render::DirectionalLight& dirLight);

  // id must be unique for any emitter (e.g entity id + emitter index in its system)
  void submitParticleEmitter(uint32_t id, const ParticleSystemRenderInfo& emitter);

  inline const glm::uvec2& getFrameSize() const
  {
    return m_frameSize;
  }
  void setFrameSize(const glm::uvec2& s);

  inline RenderPipeline& getRenderPipeline()
  {
    return m_pipeline;
  }

private:

  void updateFinalTextureWithWatched();

private:

  glm::uvec2 m_frameSize = { 1, 1 };

  const RenderLayout* m_renderLayout = nullptr;

  RenderPipeline m_pipeline;

  MeshRegistry            m_meshRegistry;
  LightRegistry           m_LightRegistry;
  const render::Cubemap*  m_skybox;
  ParticleEmitterRegistry m_particleEmitterRegistry;
};

} // namespace vrm
