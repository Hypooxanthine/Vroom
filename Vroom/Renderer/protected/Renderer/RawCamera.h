#pragma once

#include "Renderer/Api.h"
#include "Renderer/CameraBasic.h"

namespace vrm
{

class VRM_RENDERER_API RawCamera : public CameraBasic
{
public:

  RawCamera();
  virtual ~RawCamera() = default;

  /* GETTERS */

  /* Setters */

  void setViewMatrix(const glm::mat4& view);
  void setProjectionMatrix(const glm::mat4& projection);

protected:

  virtual glm::mat4 onViewComputed() const;
  virtual glm::mat4 onProjectionComputed() const;
};

} // namespace vrm
