#pragma once

#include "Vroom/Event/Codes.h"

#include "Vroom/DataStructure/TwoWaysUnorderedMap.h"

namespace vrm
{

class GLFWEventsConverter
{
public:

    /**
     * @brief Will initialize static mappers of event codes.
     * Has to be called before using conversion functions.
     */
    static void Init();

    /**
     * @brief Get Vroom key code from a GLFW key code. Needs GLFWEventsConverter::Init() to be called first.
     * @param GLFWCode GLFW key code.
     * @return Vroom key code.
     */
    static KeyCode GetKeyCodeFromGLFW(int GLFWCode);

    /**
     * @brief Get Vroom mouse code from a GLFW mouse code. Needs GLFWEventsConverter::Init() to be called first.
     * @param GLFWCode GLFW mouse code.
     * @return Vroom mouse code.
     */
    static MouseCode GetMouseCodeFromGLFW(int GLFWCode);

    /**
     * @brief Get GLFW key code from a Vroom key code. Needs GLFWEventsConverter::Init() to be called first.
     * @param code Vroom key code.
     * @return GLFW key code.
     */
    static int GetGLFWFromKeyCode(const KeyCode& code);

    /**
     * @brief Get GLFW mouse code from a Vroom key code. Needs GLFWEventsConverter::Init() to be called first.
     * @param code Vroom key code.
     * @return GLFW mouse code.
     */
    static int GetGLFWFromMouseCode(const MouseCode& code);

private:
    static TwoWaysUnorderedMap<int, KeyCode> s_KeyCodeMapper;
    static TwoWaysUnorderedMap<int, MouseCode> s_mouseCodeMapper;
};

} // namespace vrm
