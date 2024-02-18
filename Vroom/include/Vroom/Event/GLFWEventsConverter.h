#pragma once

#include "Vroom/Event/Codes.h"

#include "Vroom/DataStructure/TwoWaysUnorderedMap.h"

namespace vrm
{

class GLFWEventsConverter
{
public:
    static void Init();

    static KeyCode GetKeyCodeFromGLFW(int GLFWCode);
    static MouseCode GetMouseCodeFromGLFW(int GLFWCode);

    static int GetGLFWFromKeyCode(const KeyCode& code);
    static int GetGLFWFromMouseCode(const MouseCode& code);

private:
    static TwoWaysUnorderedMap<int, KeyCode> s_KeyCodeMapper;
    static TwoWaysUnorderedMap<int, MouseCode> s_mouseCodeMapper;
};

} // namespace vrm
