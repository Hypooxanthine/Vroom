#pragma once

#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderViewport.h"

namespace vrm
{

  using NormalizedViewport = render::ViewportBase<float>;
  
  class RenderView
  {
  public:

    RenderView() = delete;

    RenderView(CameraBasic* camera, const NormalizedViewport& viewport)
      : m_camera(camera), m_viewport(viewport)
    {}

    RenderView(CameraBasic* camera)
      : RenderView(camera, NormalizedViewport({ 0.f, 0.f }, { 1.f, 1.f }))
    {}

    RenderView& operator=(const RenderView& other) = default;
    RenderView(const RenderView& other) = default;

    RenderView& operator=(RenderView&& other) = default;
    RenderView(RenderView&& other) = default;

    inline CameraBasic* getCamera() const { return m_camera; }
    inline const NormalizedViewport& getViewport() const { return m_viewport; }

    inline void setCamera(CameraBasic* camera) { m_camera = camera; }
    inline void setViewport(const NormalizedViewport& viewport) { m_viewport = viewport; }

  private:

    CameraBasic* m_camera;
    NormalizedViewport m_viewport;

  };

} // namespace vrm
