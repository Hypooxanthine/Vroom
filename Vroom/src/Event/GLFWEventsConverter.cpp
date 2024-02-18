#include "Vroom/Event/GLFWEventsConverter.h"

#include <GLFW/glfw3.h>

#include <unordered_map>

namespace vrm
{

const std::unordered_map<int, KeyCode> KEY_CODE_MAPPER = {
    { GLFW_KEY_0, KeyCode::NUMPAD_0 },
    { GLFW_KEY_1, KeyCode::NUMPAD_1 },
    { GLFW_KEY_2, KeyCode::NUMPAD_2 },
    { GLFW_KEY_3, KeyCode::NUMPAD_3 },
    { GLFW_KEY_4, KeyCode::NUMPAD_4 },
    { GLFW_KEY_5, KeyCode::NUMPAD_5 },
    { GLFW_KEY_6, KeyCode::NUMPAD_6 },
    { GLFW_KEY_7, KeyCode::NUMPAD_7 },
    { GLFW_KEY_8, KeyCode::NUMPAD_8 },
    { GLFW_KEY_9, KeyCode::NUMPAD_9 },
    { GLFW_KEY_A, KeyCode::A },
    { GLFW_KEY_B, KeyCode::B },
    { GLFW_KEY_C, KeyCode::C },
    { GLFW_KEY_D, KeyCode::D },
    { GLFW_KEY_E, KeyCode::E },
    { GLFW_KEY_F, KeyCode::F },
    { GLFW_KEY_G, KeyCode::G },
    { GLFW_KEY_H, KeyCode::H },
    { GLFW_KEY_I, KeyCode::I },
    { GLFW_KEY_J, KeyCode::J },
    { GLFW_KEY_K, KeyCode::K },
    { GLFW_KEY_L, KeyCode::L },
    { GLFW_KEY_M, KeyCode::M },
    { GLFW_KEY_N, KeyCode::N },
    { GLFW_KEY_O, KeyCode::O },
    { GLFW_KEY_P, KeyCode::P },
    { GLFW_KEY_Q, KeyCode::Q },
    { GLFW_KEY_R, KeyCode::R },
    { GLFW_KEY_S, KeyCode::S },
    { GLFW_KEY_T, KeyCode::T },
    { GLFW_KEY_U, KeyCode::U },
    { GLFW_KEY_V, KeyCode::V },
    { GLFW_KEY_W, KeyCode::W },
    { GLFW_KEY_X, KeyCode::X },
    { GLFW_KEY_Y, KeyCode::Y },
    { GLFW_KEY_Z, KeyCode::Z },
    { GLFW_KEY_SPACE, KeyCode::Space },
    { GLFW_KEY_ESCAPE, KeyCode::Escape },
    { GLFW_KEY_ENTER, KeyCode::Enter },
    { GLFW_KEY_TAB, KeyCode::Tab },
    { GLFW_KEY_SPACE, KeyCode::Space },
};

const std::unordered_map<int, MouseCode> MOUSE_CODE_MAPPER = {
    { GLFW_MOUSE_BUTTON_LEFT, MouseCode::Left },
    { GLFW_MOUSE_BUTTON_RIGHT, MouseCode::Right },
    { GLFW_MOUSE_BUTTON_MIDDLE, MouseCode::Middle },
    { GLFW_MOUSE_BUTTON_4, MouseCode::MB4 },
    { GLFW_MOUSE_BUTTON_5, MouseCode::MB5 },
    { GLFW_MOUSE_BUTTON_6, MouseCode::MB6 },
    { GLFW_MOUSE_BUTTON_7, MouseCode::MB7 },
    { GLFW_MOUSE_BUTTON_8, MouseCode::MB8 },
};
    
KeyCode GLFWEventsConverter::GetKeyCodeFromGLFW(int GLFWCode)
{
    if (KEY_CODE_MAPPER.contains(GLFWCode))
        return KEY_CODE_MAPPER.at(GLFWCode);

    return KeyCode::None;
}

MouseCode GLFWEventsConverter::GetMouseCodeFromGLFW(int GLFWCode)
{
    if (MOUSE_CODE_MAPPER.contains(GLFWCode))
        return MOUSE_CODE_MAPPER.at(GLFWCode);

    return MouseCode::None;
}

} // namespace vrm
