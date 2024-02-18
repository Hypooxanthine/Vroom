#pragma once

#include "Vroom/Event/Codes.h"

namespace vrm
{

class GLFWEventsConverter
{
public:
    static KeyCode GetKeyCodeFromGLFW(int GLFWCode);
    static MouseCode GetMouseCodeFromGLFW(int GLFWCode);
};

} // namespace vrm
