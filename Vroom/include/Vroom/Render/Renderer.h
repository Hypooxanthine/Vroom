#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Vroom/Render/RenderSettings.h"
#include "Vroom/Render/Passes/RenderPassManager.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
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
    static Renderer &Get();

    Renderer(const Renderer &) = delete;
    Renderer(Renderer &&) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer &operator=(Renderer &&) = delete;

    /**
     * @brief Releases GPU memory.
     */
    ~Renderer();

    void setRenderSettings(const RenderSettings& settings);

    inline const RenderSettings& getRenderSettings() const { return m_renderSettings; }

    /**
     * @brief Has to be called before any rendering of current frame.
     *
     */
    void beginScene(const CameraBasic &camera);

    /**
     * @brief Has to be called after any rendering scene.
     *
     */
    void endScene();

    void submitMesh(uint32_t id, const MeshComponent &meshComponent, const glm::mat4 *model);

    /**
     * @brief Submits a point light to be drawn.
     *
     * @warning Position and pointLightComponent must be still alive when calling endScene. This is needed because the renderer does not store the data, it only stores references to it.
     *
     * @param position  The position of the light.
     * @param pointLight  The point light component.
     * @param identifier  The identifier of the light.
     */
    void registerPointLight(const glm::vec3 &position, const PointLightComponent &pointLight, size_t identifier);

    void unregisterPointLight(size_t identifier);

    void updatePointLight(const glm::vec3 &position, const PointLightComponent &pointLight, size_t identifier);

    void registerDirectionalLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier);

    void unregisterDirectionalLight(size_t identifier);

    void updateDirectionalLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier);

    /**
     * @brief Gets the viewport origin.
     * @return The viewport origin.
     */
    const glm::vec<2, unsigned int> &getViewportOrigin() const;

    /**
     * @brief Gets the viewport size.
     * @return The viewport size.
     */
    const glm::vec<2, unsigned int> &getViewportSize() const;

    /**
     * @brief Sets viewport settings.
     * @param o New viewport origin.
     * @param s New viewport size.
     */
    void setViewport(const glm::vec<2, unsigned int> &o, const glm::vec<2, unsigned int> &s);

    /**
     * @brief Sets viewport origin.
     * @param o New viewport origin.
     */
    void setViewportOrigin(const glm::vec<2, unsigned int> &o);

    /**
     * @brief Sets viewport size.
     * @param s New viewport size.
     */
    void setViewportSize(const glm::vec<2, unsigned int> &s);

    const gl::FrameBuffer &getMainFrameBuffer() const { return m_mainFrameBuffer; }

  private:
    /**
     * @brief Initializes OpenGl settings for the engine.
     */
    Renderer();

  private:
    void createRenderPasses();

    /**
     * @brief  Draws a mesh with a material and a transform.
     *
     * @param mesh  The mesh to draw.
     * @param mat The material.
     * @param model  The model matrix.
     */
    void drawMesh(const RenderMesh &mesh) const;

  private:
    static std::unique_ptr<Renderer> s_Instance;

    RenderSettings m_renderSettings;

    glm::uvec2 m_ViewportOrigin = {0, 0};
    glm::uvec2 m_ViewportSize = {1, 1};

    const CameraBasic *m_Camera = nullptr;

    bool m_passManagerDirty = false;
    RenderPassManager m_passManager;

    gl::FrameBuffer m_mainFrameBuffer;

    MeshRegistry m_meshRegistry;

    LightRegistry m_LightRegistry;
    ClusteredLights m_ClusteredLights;

    std::vector<std::unique_ptr<gl::FrameBuffer>> m_frameBufferPool;
  };

} // namespace vrm