#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "Vroom/Render/Helpers/ResourcePool.h"

#include "Vroom/Render/GPURuntimeFeatures.h"
#include "Vroom/Render/RenderSettings.h"
#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/Passes/RenderPassManager.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/Abstraction/OwningFrameBuffer.h"
#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Clustering/LightRegistry.h"
#include "Vroom/Render/Clustering/ClusteredLights.h"

namespace vrm
{

  class Application;
  class Scene;
  struct PointLightComponent;
  struct DirectionalLightComponent;
  class MeshComponent;
  class CameraBasic;

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
    void beginScene(const CameraBasic& camera);

    /**
     * @brief Has to be called after any rendering scene.
     *
     */
    void endScene();

    // Id must be a reliable value to identify the mesh component
    void submitMesh(uint32_t id, const MeshComponent& meshComponent, const glm::mat4* model);

    void submitPointLight(size_t id, const PointLightComponent& pointLight, const glm::vec3& position);

    void submitDirectionalLight(size_t id, const DirectionalLightComponent& dirLight, const glm::vec3& direction);

    /**
     * @brief Gets the viewport origin.
     * @return The viewport origin.
     */
    const glm::uvec2& getViewportOrigin() const;

    /**
     * @brief Gets the viewport size.
     * @return The viewport size.
     */
    const glm::uvec2& getViewportSize() const;

    /**
     * @brief Sets viewport settings.
     * @param o New viewport origin.
     * @param s New viewport size.
     */
    void setViewport(const glm::uvec2& o, const glm::uvec2& s);

    /**
     * @brief Sets viewport origin.
     * @param o New viewport origin.
     */
    void setViewportOrigin(const glm::uvec2& o);

    /**
     * @brief Sets viewport size.
     * @param s New viewport size.
     */
    void setViewportSize(const glm::uvec2& s);

    const gl::Texture* getRenderedTexture() const { return m_finalTexture; }

    const ResourcePool<vrm::gl::Texture>& getTexturesPool() const { return m_texturePool; }
    const gl::Texture* getTexture(const std::string& texName) const;

    uint32_t getEntityIndexOnPixel(const glm::ivec2& px) const;

  private:

    Renderer();

  private:
    void createRenderPasses();

  private:
    static std::unique_ptr<Renderer> s_Instance;

    GPURuntimeFeatures m_gpuFeatures;
    RenderSettings m_renderSettings;
    DynamicRenderSettings m_dynamicSettings;

    RenderViewport m_viewport = { { 0, 0 }, { 1, 1} };

    const CameraBasic* m_Camera = nullptr;

    bool m_passManagerDirty = false;
    RenderPassManager m_passManager;

    gl::FrameBuffer* m_renderFrameBuffer = nullptr;
    gl::Texture* m_finalTexture = nullptr;

    MeshRegistry m_meshRegistry;

    LightRegistry m_LightRegistry;
    ClusteredLights m_ClusteredLights;

    ResourcePool<gl::FrameBuffer> m_frameBufferPool;
    ResourcePool<gl::Texture> m_texturePool;
    ResourcePool<gl::Buffer> m_buffersPool; 
    ResourcePool<render::AutoBuffer> m_autoBuffersPool;
  };

} // namespace vrm