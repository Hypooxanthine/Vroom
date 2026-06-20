#include "Renderer/RawCamera.h"
#include "Renderer/CameraBasic.h"

using namespace vrm;

RawCamera::RawCamera(const glm::mat4& view, const glm::mat4& projection)
{
  setViewMatrix(view);
  setProjectionMatrix(projection);
}

RawCamera::RawCamera(const CameraBasic& cameraBasic)
  : vrm::RawCamera(cameraBasic.getView(), cameraBasic.getProjection())
{}

void RawCamera::setViewMatrix(const glm::mat4& view)
{
  m_View = view;
  updateViewProjection();
}

void RawCamera::setProjectionMatrix(const glm::mat4& projection)
{
  m_Projection = projection;
  updateViewProjection();

  // Recover the near/far planes from the projection so getNear()/getFar() stay
  // meaningful for shaders that read them (e.g. depth linearization).
  const glm::mat4 projInv = glm::inverse(projection);

  glm::vec4 nearPoint = projInv * glm::vec4(0.f, 0.f, -1.f, 1.f);
  m_Near              = nearPoint.z / nearPoint.w;

  glm::vec4 farPoint = projInv * glm::vec4(0.f, 0.f, 1.f, 1.f);
  m_Far              = farPoint.z / farPoint.w;
}
