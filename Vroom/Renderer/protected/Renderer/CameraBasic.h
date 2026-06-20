#pragma once

#include <glm/glm.hpp>
#include "Renderer/Api.h"

namespace vrm
{

  /**
   * @brief Interface shared by every camera kind.
   *
   * It only promises that a camera can expose a view, a projection, their
   * product and a world-space position.
   */
  class VRM_RENDERER_API CameraBasic
  {
  public:
    CameraBasic() = default;
    virtual ~CameraBasic() = default;

    virtual float getNear() const = 0;
    virtual float getFar() const = 0;

    virtual const glm::mat4& getView() const = 0;
    virtual const glm::mat4& getProjection() const = 0;
    virtual const glm::mat4& getViewProjection() const = 0;

    /**
     * @brief World-space position of the camera.
     *
     * The default implementation recovers it from the view matrix (translation
     * column of its inverse), which is correct for any view. Subclasses that
     * already store a position should override this for precision and speed.
     */
    virtual glm::vec3 getPosition() const;

    virtual void setViewportSize(float width, float height) {}

    glm::vec3 getForwardVector() const;
    glm::vec3 getUpVector() const;
    glm::vec3 getRightVector() const;
  };

} // namespace vrm
