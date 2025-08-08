#include "Vroom/Render/RenderView.h"

using namespace vrm::render;

View::View(const NormalizedView& view, const glm::uvec2& frameSize)
  : ViewBase(view.getCamera())
{
  glm::vec2 frameSizeFloat = glm::vec2(frameSize);

  glm::vec2 originFloat = view.getViewport().getOrigin() * frameSizeFloat;
  glm::vec2 sizeFloat = view.getViewport().getSize() * frameSizeFloat;

  setViewport(Viewport(glm::uvec2(originFloat), glm::uvec2(sizeFloat)));
}
