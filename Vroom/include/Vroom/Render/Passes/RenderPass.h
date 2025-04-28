#pragma once

namespace vrm::gl
{
  class Shader;
}

namespace vrm
{

  struct RenderPassContext;
  class CameraBasic;
  class RenderViewport;

  class RenderPass
  {
  public:
    RenderPass();
    virtual ~RenderPass();

    void init();

    void setup(const RenderPassContext& ctx);

    void render(const RenderPassContext& ctx) const;

    void cleanup(const RenderPassContext& ctx);

  protected:
    /**
     * @brief Called once at the creation of the render pass
     *
     */
    virtual void onInit() {}

    /**
     * @brief Called for all at the setup stage of the frame
     *
     */
    virtual void onSetup(const RenderPassContext& ctx) {}

    /**
     * @brief Called for all passes at the rendering stage of the frame
     * 
     */
    virtual void onRender(const RenderPassContext& ctx) const = 0;

    /**
     * @brief Called for all passes at the cleanup stage of the frame
     * 
     */

    virtual void onCleanup(const RenderPassContext& ctx) {}

  protected: // Helper functions

    static void applyCameraUniforms(const gl::Shader& shader, const CameraBasic& camera);

    static void applyViewportUniforms(const gl::Shader& shader, const RenderViewport& viewport);

  private:
  };

  struct RenderPassContext
  {
    const CameraBasic* mainCamera = nullptr;
  };

} // namespace vrm
