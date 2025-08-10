#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Vroom/Render/Passes/RenderPass.h"
#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/MeshTag.h"
#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::gl
{
  class Shader;
  class FrameBuffer;
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

    struct StorageBufferParameter
    {
      std::string name;
      const gl::Buffer* buffer;
      size_t offset = 0, size = 0;

      StorageBufferParameter(const std::string& name, const gl::Buffer* buffer)
        : StorageBufferParameter(name, buffer, 0, 0)
      {}

      StorageBufferParameter(const std::string& name, const gl::Buffer* buffer, size_t offset, size_t size)
        : name(name), buffer(buffer), offset(offset), size(size)
      {}
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

    const gl::Buffer* clusteredLightsBuffer = nullptr;
    size_t clusteredLightPerViewSize = 0;
    std::vector<StorageBufferParameter> storageBufferParameters;

  protected:

    virtual void onInit() override;

    virtual void onSetup(const RenderPassContext& ctx) override;

    virtual void onRender(const RenderPassContext& ctx) const override;

    void setupFaceCulling() const;

    void renderMeshes(const RenderPassContext& ctx) const;

    void applyStorageBufferParameters(const gl::Shader& shader) const;
  };

} // namespace vrm 
