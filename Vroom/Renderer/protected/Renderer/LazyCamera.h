#pragma once

#include <glm/glm.hpp>
#include "Renderer/Api.h"
#include "Renderer/CameraBasic.h"

namespace vrm
{

  /**
   * @brief Base for "parametric" cameras whose view and projection are derived
   * from parameters (position, orientation, fov, ...).
   *
   * The matrices are computed lazily through the onViewComputed() /
   * onProjectionComputed() hooks and cached until a parameter setter marks them
   * dirty. Subclasses must call markViewDirty() / markProjectionDirty() whenever
   * a parameter they own changes.
   *
   * This base is only concerned with the lazy view/projection caching. Other
   * camera attributes (near/far planes, position, ...) are left to the concrete
   * cameras, since not every parametric camera expresses them the same way.
   */
  class VRM_RENDERER_API LazyCamera : public CameraBasic
  {
  public:
    LazyCamera() = default;

    const glm::mat4& getView() const override;
    const glm::mat4& getProjection() const override;
    const glm::mat4& getViewProjection() const override;

  protected:
    void markViewDirty() { m_ViewDirty = true; m_ViewProjectionDirty = true; }
    void markProjectionDirty() { m_ProjectionDirty = true; m_ViewProjectionDirty = true; }

    virtual glm::mat4 onViewComputed() const = 0;
    virtual glm::mat4 onProjectionComputed() const = 0;

  private:
    void computeView() const;
    void computeProjection() const;
    void computeViewProjection() const;

  private:
    mutable glm::mat4 m_View,
      m_Projection,
      m_ViewProjection;
    mutable bool m_ViewDirty = true,
      m_ProjectionDirty = true,
      m_ViewProjectionDirty = true;
  };

} // namespace vrm
