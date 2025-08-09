#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "Vroom/Render/RenderResources.h"

#include "Vroom/Render/GPURuntimeFeatures.h"
#include "Vroom/Render/RenderSettings.h"
#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/Passes/RenderPassManager.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/RenderObject/RenderSkybox.h"
#include "Vroom/Render/Abstraction/OwningFrameBuffer.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Clustering/LightRegistry.h"

namespace vrm
{

  class Application;
  class Scene;
  struct PointLightComponent;
  struct DirectionalLightComponent;
  class MeshComponent;
  class SkyboxComponent;
  class RenderLayout;

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
    /**
     * @brief Initializes the renderer.
     */
    static void Init();

    /**
     * @brief Shuts down the renderer.
     */
    static void Shutdown();

    /**
     * @brief Gets the renderer instance.
     * @return The renderer instance.
     */
    static Renderer& Get();

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    /**
     * @brief Releases GPU memory.
     */
    ~Renderer();

    inline const GPURuntimeFeatures& getGPUFeatures() const { return m_gpuFeatures; }

    void setRenderSettings(const RenderSettings& settings);
    inline const RenderSettings& getRenderSettings() const { return m_renderSettings; }

    void setDynamicRenderSettings(const DynamicRenderSettings& settings);
    inline const DynamicRenderSettings& getDynamicRenderSettings() const { return m_dynamicSettings; }

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

    inline const glm::uvec2& getFrameSize() const { return m_frameSize; }
    void setFrameSize(const glm::uvec2& s);

    const gl::Texture* getRenderedTexture() const { return m_finalTexture; }
    const std::vector<std::string>& getExposedTextureNames() const { return m_resources.getExposedTextures(); }
    void watchExposedTexture(const std::string& name);
    const std::string& getWatchedTexture() const { return m_watchedTexture; }

    uint32_t getEntityIndexOnPixel(const glm::ivec2& px) const;

  private:

    Renderer();

  private:
    void createRenderPasses();
    void updateFinalTextureWithWatched();

  private:
    static std::unique_ptr<Renderer> s_Instance;

    GPURuntimeFeatures m_gpuFeatures;
    RenderSettings m_renderSettings;
    DynamicRenderSettings m_dynamicSettings;

    glm::uvec2 m_frameSize = { 1, 1 };

    const RenderLayout* m_renderLayout = nullptr;

    bool m_passManagerDirty = false;
    RenderPassManager m_passManager;

    gl::FrameBuffer* m_renderFrameBuffer = nullptr;
    gl::Texture* m_finalTexture = nullptr;
    std::string m_watchedTexture = "";

    MeshRegistry m_meshRegistry;
    RenderSkybox m_skybox;

    LightRegistry m_LightRegistry;

    RenderResources m_resources;
  };

} // namespace vrm