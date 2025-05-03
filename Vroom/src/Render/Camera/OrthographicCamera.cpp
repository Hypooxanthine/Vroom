#include "Vroom/Render/Camera/OrthographicCamera.h"


#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace vrm;

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
  : CameraBasic(near, far), m_left(left), m_right(right), m_bottom(bottom), m_top(top)
{
}

OrthographicCamera::OrthographicCamera(float width, float height, float near, float far)
  : OrthographicCamera(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, near, far)
{

}

// OrthographicCamera::OrthographicCamera(const OrthographicCamera& other)
//   : CameraBasic(other)
// {
//   m_left = other.m_left;
//   m_right = other.m_right;
//   m_bottom = other.m_bottom;
//   m_top = other.m_top;
//   m_worldPosition = other.m_worldPosition;
//   m_viewDir = other.m_viewDir;
// }

OrthographicCamera::~OrthographicCamera()
{

}

void OrthographicCamera::setWorldPosition(const glm::vec3& pos)
{
  m_worldPosition = pos;
  markViewDirty();
}

void OrthographicCamera::setViewDir(const glm::vec3& viewDir)
{
  m_viewDir = viewDir;
  markViewDirty();
}

void OrthographicCamera::setViewportSize(float width, float height)
{
  m_left = -width / 2.f;
  m_right = width / 2.f;
  m_bottom = -height / 2.f;
  m_top = height / 2.f;

  markProjectionDirty();
}

glm::mat4 OrthographicCamera::onViewComputed() const
{
  return glm::lookAt(m_worldPosition, m_worldPosition + m_viewDir, glm::vec3 { 0.f, 1.f, 0.f });
}

glm::mat4 OrthographicCamera::onProjectionComputed() const
{
  return glm::ortho(m_left, m_right, m_bottom, m_top, getNear(), getFar());
}
