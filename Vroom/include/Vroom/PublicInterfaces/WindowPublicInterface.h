#pragma once

#include "Vroom/Core/Window.h"

namespace vrm
{

/**
 * @brief Public interface of the Window class.
 * 
 * This class is used to access the public interface of the Window class.
 * This was just a way to avoid exposing the whole Window class to the user, but I think I may rework all of this with a PRIVATE/PUBLIC api system.
 * 
 */
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
