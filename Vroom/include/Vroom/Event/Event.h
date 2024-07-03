#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

struct Event
{
    enum class Type
    { 
        None = 0,
        KeyPressed, KeyReleased,
        MousePressed, MouseReleased, Scroll,
        MouseMoved, MouseEntered, MouseLeft,
        WindowsResized, GainedFocus, LostFocus,
        Exit,
    };

    Type type = Type::None;

    union
    {
        struct
        {
            double mouseX, mouseY, mouseDeltaX, mouseDeltaY;
        };

        struct
        {
            double scrollX, scrollY;
        };

        struct
        {
            int newWidth, newHeight;
        };
    };

    union
    {
        CodeType standardizedCode = VRM_EVENT_CODE_NONE;
        KeyCode keyCode;
        MouseCode mouseCode;
    };
    
};

} // namespace vrm
