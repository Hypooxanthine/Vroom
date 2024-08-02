#include "Vroom/Core/Layer.h"

namespace vrm
{

void Layer::init()
{
    onInit();
}

void Layer::end()
{
    onEnd();
}

void Layer::update(float dt)
{
    if (m_ShouldUpdate)
        onUpdate(dt);
}

void Layer::render()
{
    if (m_ShouldRender)
        onRender();
}

void Layer::submitEvent(Event& e)
{
    if (m_ShouldHandleEvents)
        onEvent(e);
}

} // namespace vr