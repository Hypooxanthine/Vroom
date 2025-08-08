#include "Vroom/Render/RenderView.h"

using namespace vrm;

render::Viewport RenderView::getViewport(const glm::uvec2& frameSizePx) const
{
  glm::vec2 frameSizeFloat = glm::vec2(frameSizePx);

  glm::vec2 originFloat = m_viewport.getOrigin() * frameSizeFloat;
  glm::vec2 sizeFloat = m_viewport.getSize() * frameSizeFloat;

  return render::Viewport(glm::uvec2(originFloat), glm::uvec2(sizeFloat));
}