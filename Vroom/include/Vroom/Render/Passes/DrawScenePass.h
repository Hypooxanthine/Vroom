#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#include "Vroom/Render/MeshRegistry.h"
#include "Vroom/Render/Passes/RenderPass.h"

namespace vrm::gl
{
  class Shader;
  class FrameBuffer;
  class StorageBufferBase;
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
    glm::uvec2 *viewportOrigin, *viewportSize;
    const CameraBasic** camera = nullptr;
    EFaceCulling faceCulling = EFaceCulling::eNone;
    EFrontFace frontFace = EFrontFace::eCCW;

    std::unordered_map<std::string, const gl::StorageBufferBase*> storageBufferParameters;

  protected:

    virtual void onRender() const override;

    void setupFaceCulling() const;

    void renderMeshes() const;

    void applyCameraUniforms(const gl::Shader& shader) const;

    void applyStorageBufferParameters(const gl::Shader& shader) const;
  };

} // namespace vrm 
