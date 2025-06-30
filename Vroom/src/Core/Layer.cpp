#include "Vroom/Core/Layer.h"

#include "Vroom/Core/Assert.h"

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
  _consumeRountines(EFrameLocation::ePreNewFrame);

  onNewFrame();

  _consumeRountines(EFrameLocation::ePostNewFrame);
}

void Layer::update(const DeltaTime& dt)
{
  _consumeRountines(EFrameLocation::ePreUpdate);

  if (m_ShouldUpdate || m_ForceUpdateNextFrame)
  {
    m_ForceUpdateNextFrame = false;
    onUpdate(dt);
  }

  _consumeRountines(EFrameLocation::ePostUpdate);
}

void Layer::render()
{
  if (m_ShouldRender || m_ForceRenderNextFrame)
  {
    m_ForceRenderNextFrame = false;
    onRender();
  }
}

void Layer::endFrame()
{
  _consumeRountines(EFrameLocation::ePreEndFrame);

  onEndFrame();

  _consumeRountines(EFrameLocation::ePostEndFrame);
}

void Layer::submitEvent(Event &e)
{
  if (m_ShouldHandleEvents)
    onEvent(e);
}

void Layer::pushRoutine(const DeferredRoutine& routine, EFrameLocation::Type location)
{
  VRM_ASSERT(location >= 0 && location < EFrameLocation::eCount);
  m_deferredRountines.at(location).push_back(routine);
}

void Layer::forceUpdateNextFrame()
{
  m_ForceUpdateNextFrame = true;
}

void Layer::forceRenderNextFrame()
{
  m_ForceRenderNextFrame = true;
}

void Layer::_consumeRountines(EFrameLocation::Type location)
{
  auto& routines = m_deferredRountines.at(location);

  if (routines.size() > 0)
  {
    for (auto& routine : routines)
    {
      routine();
    }

    routines.clear();
  }
}
