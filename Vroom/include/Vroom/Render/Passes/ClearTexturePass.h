#pragma once

#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Passes/RenderPass.h"

namespace vrm
{

  class CameraBasic;
  
  class ClearTexturePass : public RenderPass
  {
  public:
    
    ClearTexturePass();
    virtual ~ClearTexturePass();

  public:

    GLuint texture = 0;
    GLint level = 0;
    GLenum format = 0;
    GLenum type = 0;
    const void * clearValue = nullptr;

  protected:

    virtual void onInit() override;

    virtual void onRender(const RenderPassContext& ctx) const override;
  };

} // namespace vrm 
