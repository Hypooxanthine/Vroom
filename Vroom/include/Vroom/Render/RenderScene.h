#pragma once

#include "Vroom/Render/MeshRegistry.h"

namespace vrm
{
  
  class RenderScene
  {
  public:
    
    RenderScene();
    ~RenderScene();

    RenderScene& operator=(const RenderScene& other) = delete;
    RenderScene(const RenderScene& other) = delete;
    
    RenderScene& operator=(RenderScene&& other) = delete;
    RenderScene(RenderScene&& other) = delete;

  private:

    MeshRegistry m_meshRegistry;
  };

}