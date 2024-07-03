#pragma once

#include "Vroom/Core/Window.h"

namespace vrm
{

class WindowPublicInterface
{
public:
    WindowPublicInterface(Window& window);
    
    // Getters
    bool getKeyRepeatEnabled() const;
    bool getCursorVisible() const;

    // Setters
    void setKeyRepeatEnabled(bool keyRepeat);
    void setCursorVisible(bool visible);

    // Events
    bool isKeyPressed(const KeyCode& code) const;
    bool isMousePressed(const MouseCode& code) const;

private:
    Window& m_Window;
};

} // namespace vrm
