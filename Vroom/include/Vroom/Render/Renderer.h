#pragma once

#include <glm/glm.hpp>

#include "Vroom/Api.h"

#include "Vroom/Render/GPURuntimeFeatures.h"
#include "Vroom/Render/ParticleEmitterRegistry.h"
#include "Vroom/Render/RenderPipeline.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/RenderObject/RenderSkybox.h"
#include "Vroom/Render/Clustering/LightRegistry.h"

namespace vrm
{

  class Application;
  class Scene;
  class PointLightComponent;
  struct DirectionalLightComponent;
  class MeshComponent;
  class SkyboxComponent;
  class RenderLayout;
  class ParticleSystemComponent;

  namespace gl
  {
    class FrameBuffer;
  }

  /**
   * @brief The renderer is responsible for rendering objects on the scene, taking lights and cameras into consideration.
   *
   */
  class VRM_API Renderer
  {
  public:

    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    /**
     * @brief Releases GPU memory.
     */
    ~Renderer();

    inline const GPURuntimeFeatures& getGPUFeatures() const { return s_gpuFeatures; }

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

    // Id must be a reliable value to identify the mesh component
    void submitMesh(uint32_t id, const MeshComponent& meshComponent, const glm::mat4* model);

    void submitSkybox(const SkyboxComponent& skyboxComponent);

    void submitPointLight(size_t id, const PointLightComponent& pointLight, const glm::vec3& position);

    void submitDirectionalLight(size_t id, const DirectionalLightComponent& dirLight, const glm::vec3& direction);

    void submitParticleSystem(uint32_t id, const ParticleSystemComponent& system, const glm::mat4* model);

    inline const glm::uvec2& getFrameSize() const { return m_frameSize; }
    void setFrameSize(const glm::uvec2& s);

    inline RenderPipeline& getRenderPipeline() { return m_pipeline; }

  private:
    void updateFinalTextureWithWatched();

  private:
    static GPURuntimeFeatures s_gpuFeatures;

    glm::uvec2 m_frameSize = { 1, 1 };

    const RenderLayout* m_renderLayout = nullptr;

    RenderPipeline m_pipeline;

    MeshRegistry m_meshRegistry;
    LightRegistry m_LightRegistry;
    RenderSkybox m_skybox;
    ParticleEmitterRegistry m_particleEmitterRegistry;

  };

} // namespace vrm