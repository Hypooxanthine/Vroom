#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/MeshRegistry.h"

namespace vrm::gl
{
  class Shader;
  class FrameBuffer;
  class StorageBufferBase;
  class ArrayTexture2D;
}

namespace vrm
{

  class CameraBasic;
  
  class DrawSceneRenderPass : public RenderPass
  {
  public:

    enum class EFaceCulling
    {
      eNone = 0, eCullFront, eCullBack
    };

    enum class EFrontFace
    {
      eCW = 0, eCCW
    };

  public:
    
    DrawSceneRenderPass();
    virtual ~DrawSceneRenderPass();

  public:

    const MeshRegistry* meshRegistry = nullptr;
    gl::FrameBuffer* framebufferTarget = nullptr;
    const RenderViewport* viewport = nullptr;
    const gl::ArrayTexture2D* dirLightShadowMaps = nullptr;
    EFaceCulling faceCulling = EFaceCulling::eNone;
    EFrontFace frontFace = EFrontFace::eCCW;
    bool shadowsEnable = true;
    uint8_t softShadowKernelRadius = 2;
    gl::Texture2D* entityPickingTex = nullptr;

    std::unordered_map<std::string, const gl::StorageBufferBase*> storageBufferParameters;

  protected:

    virtual void onRender(const RenderPassContext& ctx) const override;

    void setupFaceCulling() const;

    void renderMeshes(const RenderPassContext& ctx) const;

    void applyStorageBufferParameters(const gl::Shader& shader) const;
  };

} // namespace vrm 
