#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

struct Event
{
    bool keyEvent = false;
    bool keyPressed = false;
    bool keyReleased = false;

    bool mouseEvent = false;
    bool mouseButtonPressed = false;
    bool mouseButtonReleased = false;

    bool scrollEvent = false;
    double scrollY = 0;
    double scrollX = 0;

    bool resizeEvent = false;
    int newWidth = 0, newHeight = 0;

    KeyCode keyCode = KeyCode::None;
    MouseCode mouseCode = MouseCode::None;
};

} // namespace vrm
