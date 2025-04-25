#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Vroom/Render/GPURuntimeFeatures.h"
#include "Vroom/Render/RenderSettings.h"
#include "Vroom/Render/Passes/RenderPassManager.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/Abstraction/OwningFrameBuffer.h"
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
    class ArrayTexture2D;
  }

  /**
   * @brief The renderer is responsible for rendering objects on the scene, taking lights and cameras into consideration.
   *
   */
  class Renderer
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

    const gl::OwningFrameBuffer& getMainFrameBuffer() const { return m_mainFrameBuffer; }

  private:

    Renderer();

  private:
    void createRenderPasses();

  private:
    static std::unique_ptr<Renderer> s_Instance;

    GPURuntimeFeatures m_gpuFeatures;
    RenderSettings m_renderSettings;

    glm::uvec2 m_ViewportOrigin = { 0, 0 };
    glm::uvec2 m_ViewportSize = { 1, 1 };

    const CameraBasic* m_Camera = nullptr;

    bool m_passManagerDirty = false;
    RenderPassManager m_passManager;

    gl::OwningFrameBuffer m_mainFrameBuffer;

    MeshRegistry m_meshRegistry;

    LightRegistry m_LightRegistry;
    ClusteredLights m_ClusteredLights;

    std::vector<std::unique_ptr<gl::FrameBuffer>> m_frameBufferPool;
    std::vector<std::unique_ptr<gl::ArrayTexture2D>> m_arrayTexture2DPool;
  };

} // namespace vrm