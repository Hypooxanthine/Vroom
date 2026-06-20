#include "Renderer/RawCamera.h"
#include "Renderer/CameraBasic.h"
#include "glm/fwd.hpp"
#include "glm/gtx/hash.hpp"

using namespace vrm;

RawCamera::RawCamera()
{}

glm::mat4 RawCamera::onViewComputed() const
{
  // This should never been called at all,
  // RawCamera never marks view dirty.
  return {};
}

glm::mat4 RawCamera::onProjectionComputed() const
{
  // This should never been called at all,
  // RawCamera never marks projection dirty.
  return {};
}

void RawCamera::setViewMatrix(const glm::mat4& view)
{
  CameraBasic::setView(view);
}

void RawCamera::setProjectionMatrix(const glm::mat4& projection)
{
  glm::vec4 nearPoint = { 0.f, 0.f, -1.f, 1.f };
  glm::vec4 farPoint  = { 0.f, 0.f, +1.f, 1.f };

  glm::mat4 projInv  = glm::inverse(projection);
  nearPoint          = projInv * nearPoint;
  nearPoint         /= nearPoint.w;
  setNear(nearPoint.z);

  farPoint  = projInv * farPoint;
  farPoint /= farPoint.w;
  setFar(farPoint.z);

  CameraBasic::setProjection(projection);
}
