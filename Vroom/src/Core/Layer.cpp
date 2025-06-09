#include "Vroom/Core/Layer.h"

using namespace vrm;

void Layer::init()
{
  onInit();
}

void Layer::end()
{
  onEnd();
}

void Layer::newFrame()
{
  onNewFrame();
}

void Layer::update(const DeltaTime& dt)
{
  if (m_ShouldUpdate || m_ForceUpdateNextFrame)
  {
    m_ForceUpdateNextFrame = false;
    onUpdate(dt);
  }
}

void Layer::render()
{
  if (m_ShouldRender || m_ForceRenderNextFrame)
  {
    m_ForceRenderNextFrame = false;
    onRender();
  }
}

void Layer::submitEvent(Event &e)
{
  if (m_ShouldHandleEvents)
    onEvent(e);
}

void Layer::forceUpdateNextFrame()
{
  m_ForceUpdateNextFrame = true;
}
