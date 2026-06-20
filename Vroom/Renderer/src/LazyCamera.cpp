#include "Renderer/LazyCamera.h"

using namespace vrm;

const glm::mat4& LazyCamera::getView() const
{
  computeView();
  return m_View;
}

const glm::mat4& LazyCamera::getProjection() const
{
  computeProjection();
  return m_Projection;
}

const glm::mat4& LazyCamera::getViewProjection() const
{
  computeViewProjection();
  return m_ViewProjection;
}

void LazyCamera::computeView() const
{
  if (!m_ViewDirty) return;

  m_View = onViewComputed();

  m_ViewDirty = false;
}

void LazyCamera::computeProjection() const
{
  if (!m_ProjectionDirty) return;

  m_Projection = onProjectionComputed();

  m_ProjectionDirty = false;
}

void LazyCamera::computeViewProjection() const
{
  if (!m_ViewProjectionDirty) return;

  m_ViewProjection = getProjection() * getView();

  m_ViewProjectionDirty = false;
}
