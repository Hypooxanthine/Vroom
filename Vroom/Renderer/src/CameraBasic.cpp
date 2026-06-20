#include "Renderer/CameraBasic.h"

using namespace vrm;

glm::vec3 CameraBasic::getPosition() const
{
  // Translation column of the inverse view matrix.
  return glm::vec3(glm::inverse(getView())[3]);
}

glm::vec3 CameraBasic::getForwardVector() const
{
  const glm::mat4& v = getView();
  return -glm::vec3(v[0][2], v[1][2], v[2][2]);
}

glm::vec3 CameraBasic::getUpVector() const
{
  const glm::mat4& v = getView();
  return glm::vec3(v[0][1], v[1][1], v[2][1]);
}

glm::vec3 CameraBasic::getRightVector() const
{
  const glm::mat4& v = getView();
  return glm::vec3(v[0][0], v[1][0], v[2][0]);
}
