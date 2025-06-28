#pragma once

#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/MaterialDefines.h"

namespace vrm::gl
{
  class Shader;
  class FrameBuffer;
}

namespace vrm
{

  struct RenderPassContext;
  class CameraBasic;

  class RenderPass
  {
  public:
    RenderPass();
    virtual ~RenderPass();

    void init();

    void setup(const RenderPassContext& ctx);

    void render(const RenderPassContext& ctx) const;

    void cleanup(const RenderPassContext& ctx);

    inline size_t getPassIndex() const { return m_passIndex; }

    inline void addDefine(const std::string& define, const std::string& value = "")
    {
      m_defines.add(define, value);
    }
  
  public:

    friend struct ManagerAttorney;
    struct ManagerAttorney
    {
      friend class RenderPassManager;
      RenderPass* const ref;
    private:

      inline void setPassIndex(size_t id) { ref->m_passIndex = id; }

    };
    
    inline ManagerAttorney managerInterface() { return ManagerAttorney{ this }; }

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

  protected:

    MaterialDefines m_defines;

  private:

    size_t m_passIndex = 0;
  };

  struct RenderPassContext
  {
    const CameraBasic* mainCamera = nullptr;
    const gl::FrameBuffer* frameBufferTarget = nullptr;
    RenderViewport viewport;
  };

} // namespace vrm
