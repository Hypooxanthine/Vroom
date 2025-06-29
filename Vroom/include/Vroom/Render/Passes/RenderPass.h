#pragma once

#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/PassMaterials.h"

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

    template <typename T>
    inline void addDefine(const std::string& define, T&& value)
    {
      m_defines.add(define, std::forward<T>(value));
    }

    inline void addDefine(const std::string& define)
    {
      addDefine(define, 1);
    }
  
  public:

    friend struct ManagerAttorney;
    struct ManagerAttorney
    {
      friend class RenderPassManager;
      RenderPass* const ref;
    private:

      inline void setPassIndex(size_t id) { ref->m_passIndex = id; }

      inline void setPassMaterials(PassMaterials* materials) { ref->m_materialsRef = materials; }

    };
    
    inline ManagerAttorney managerInterface() { return ManagerAttorney{ this }; }

    inline const MaterialDefines& getDefines() const { return m_defines; }

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

    const PassMaterial& getPassMaterial(MaterialAsset::Handle asset, const MaterialDefines* customDefines) const;
    
    const PassMaterial& getPassMaterial(MaterialAsset::Handle asset) const;

  protected:

    MaterialDefines m_defines;

  private:

    size_t m_passIndex = 0;
    PassMaterials* m_materialsRef = nullptr;

  };

  struct RenderPassContext
  {
    const CameraBasic* mainCamera = nullptr;
    const gl::FrameBuffer* frameBufferTarget = nullptr;
    RenderViewport viewport;
  };

} // namespace vrm
