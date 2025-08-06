#pragma once

#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderViewport.h"

namespace vrm
{
  
  class RenderView
  {
  public:

    RenderView() = delete;

    RenderView(const CameraBasic* camera, const RenderViewport& viewport)
      : m_camera(camera), m_viewport(viewport)
    {}

    RenderView& operator=(const RenderView& other) = default;
    RenderView(const RenderView& other) = default;

    RenderView& operator=(RenderView&& other) = default;
    RenderView(RenderView&& other) = default;

    inline const CameraBasic* getCamera() const { return m_camera; }
    inline const RenderViewport& getViewport() const { return m_viewport; }

    inline void setCamera(const CameraBasic* camera) { m_camera = camera; }
    inline void setViewport(const RenderViewport& viewport) { m_viewport = viewport; }

  private:

    const CameraBasic* m_camera;
    RenderViewport m_viewport;

  };

} // namespace vrm
