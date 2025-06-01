#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

/**
 * @brief The Event struct is a standardized event structure that will gather all the events that can be triggered by the engine.
 * 
 * @warning Please be careful when dealing with event values (mouseX, scrollY, newWidth, ...). This struct was meant to be lightweight, so event values are stored in a union. Only the values that are relevant to the event type will be meaningful.
 * 
 */
struct Event
{
    enum class Type
    { 
        None = 0,
        KeyPressed, KeyReleased,
        MousePressed, MouseReleased, Scroll,
        MouseMoved, MouseEntered, MouseLeft,
        WindowsResized, GainedFocus, LostFocus,
        FileDragEnter, FileDragLeave, FileDrop,
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

    std::string stringData;

    union
    {
        CodeType standardizedCode = VRM_EVENT_CODE_NONE;
        KeyCode keyCode;
        MouseCode mouseCode;
    };

    mutable bool handled = false;
    
};

} // namespace vrm
