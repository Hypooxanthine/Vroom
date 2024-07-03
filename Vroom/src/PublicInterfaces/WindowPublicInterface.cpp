#include "Vroom/PublicInterfaces/WindowPublicInterface.h"

namespace vrm
{

WindowPublicInterface::WindowPublicInterface(Window& window)
    : m_Window(window)
{
}

bool WindowPublicInterface::getKeyRepeatEnabled() const
{
    return m_Window.getKeyRepeatEnabled();
}

bool WindowPublicInterface::getCursorVisible() const
{
    return m_Window.getCursorVisible();
}

void WindowPublicInterface::setKeyRepeatEnabled(bool keyRepeat)
{
    m_Window.setKeyRepeatEnabled(keyRepeat);
}

void WindowPublicInterface::setCursorVisible(bool visible)
{
    m_Window.setCursorVisible(visible);
}

bool WindowPublicInterface::isKeyPressed(const KeyCode& code) const
{
    return m_Window.isKeyPressed(code);
}

bool WindowPublicInterface::isMousePressed(const MouseCode& code) const
{
    return m_Window.isMousePressed(code);
}

} // namespace vrm