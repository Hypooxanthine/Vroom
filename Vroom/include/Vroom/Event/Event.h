#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

struct Event
{
    enum class Type { None = 0, KeyPressed, KeyReleased, MousePressed, MouseReleased, Scroll, WindowsResized, GainedFocus, LostFocus };

    Type type = Type::None;

    double scrollY = 0;
    double scrollX = 0;

    int newWidth = 0, newHeight = 0;

    KeyCode keyCode = KeyCode::None;
    MouseCode mouseCode = MouseCode::None;
};

} // namespace vrm
