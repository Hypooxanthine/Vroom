#pragma once

#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/RenderViewport.h"

namespace vrm::render
{

  using NormalizedViewport = ViewportBase<float>;

  template <typename T>
  class ViewBase
  {
  public:
  
    using ViewportType = ViewportBase<T>;

  public:

    ViewBase() = delete;

    ViewBase(CameraBasic* camera, const ViewportType& viewport)
      : m_camera(camera), m_viewport(viewport)
    {}

    ViewBase(CameraBasic* camera)
      : ViewBase(camera, ViewportType({ T(0), T(0) }, { T(1), T(1) }))
    {}

    ViewBase& operator=(const ViewBase& other) = default;
    ViewBase(const ViewBase& other) = default;

    ViewBase& operator=(ViewBase&& other) = default;
    ViewBase(ViewBase&& other) = default;

    inline CameraBasic* getCamera() const { return m_camera; }
    ViewportType getViewport() const { return m_viewport; }

    inline void setCamera(CameraBasic* camera) { m_camera = camera; }
    inline void setViewport(const ViewportType& viewport) { m_viewport = viewport; }

  private:

    CameraBasic* m_camera;
    ViewportType m_viewport;
  };
  
  using NormalizedView = ViewBase<float>;

  class View : public ViewBase<glm::uint>
  {
  public:

    View() = delete;

    View(CameraBasic* camera, const ViewportType& viewport)
      : ViewBase(camera, viewport)
    {}

    View(CameraBasic* camera)
      : ViewBase(camera)
    {}

    View(const NormalizedView& view, const glm::uvec2& frameSize);

    View& operator=(const View& other) = default;
    View(const View& other) = default;

    View& operator=(View&& other) = default;
    View(View&& other) = default;

    
  };

} // namespace vrm
