#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/RenderViewport.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/PassMaterials.h"
#include "Vroom/Render/MeshTag.h"

namespace vrm::gl
{
  class Shader;
  class FrameBuffer;
  class Buffer;
  class Texture;
}

namespace vrm
{
  
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
    const gl::Texture* dirLightShadowMaps = nullptr;
    EFaceCulling faceCulling = EFaceCulling::eNone;
    EFrontFace frontFace = EFrontFace::eCCW;
    bool shadowsEnable = false;
    bool bloomEnable = false;
    MeshTags meshTags;

    std::unordered_map<std::string, const gl::Buffer*> storageBufferParameters;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;

    void setupFaceCulling() const;

    void renderMeshes(const RenderPassContext& ctx) const;

    void applyStorageBufferParameters(const gl::Shader& shader) const;
  };

} // namespace vrm 
