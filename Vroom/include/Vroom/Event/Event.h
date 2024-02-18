#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

struct Event
{
    bool keyPressed = false;
    bool keyReleased = false;
    bool mouseButtonPressed = false;
    bool mouseButtonReleased = false;
    bool scrolled = false;

    KeyCode keyCode = KeyCode::None;
    MouseCode mouseCode = MouseCode::None;
};

} // namespace vrm
