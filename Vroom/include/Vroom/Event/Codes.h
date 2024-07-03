#pragma once

#define VRM_EVENT_CODE_NONE 0

namespace vrm
{

using CodeType = int;

enum class KeyCode : CodeType
{
    None = VRM_EVENT_CODE_NONE,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Space, Escape, Enter, Tab,
    Right, Left, Down, Up,
    NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
    LeftShift, LeftAlt, LeftCtrl, RightShift, RightAlt, RightCtrl
};

enum class MouseCode : CodeType
{
    None = VRM_EVENT_CODE_NONE,
    MB1, Left = MB1, MB2, Right = MB2, MB3, Middle = MB3,
    MB4, MB5, MB6, MB7, MB8
};

} // namespace vrm

#include <string>

namespace std
{

inline std::string to_string(const vrm::KeyCode& code)
{
    int asInt = static_cast<int>(code);

    if (asInt >= static_cast<int>(vrm::KeyCode::A) && asInt <= static_cast<int>(vrm::KeyCode::Z))
        return std::string("") + static_cast<char>('A' + static_cast<int>(code) - static_cast<int>(vrm::KeyCode::A));

    switch(code)
    {
    case vrm::KeyCode::Space:
        return "Space";
        break;
    case vrm::KeyCode::Escape:
        return "Escape";
        break;
    case vrm::KeyCode::Enter:
        return "Enter";
        break;
    case vrm::KeyCode::Tab:
        return "Tab";
        break;
    case vrm::KeyCode::Right:
        return "Right";
        break;
    case vrm::KeyCode::Left:
        return "Left";
        break;
    case vrm::KeyCode::Down:
        return "Down";
        break;
    case vrm::KeyCode::Up:
        return "Up";
        break;
    case vrm::KeyCode::LeftShift:
        return "Left Shift";
        break;
    case vrm::KeyCode::None:
        return "None";
        break;
    }

    if (asInt >= static_cast<int>(vrm::KeyCode::NUMPAD_0) && asInt <= static_cast<int>(vrm::KeyCode::NUMPAD_9))
        return std::string("Numpad ") + static_cast<char>('0' + static_cast<int>(code) - static_cast<int>(vrm::KeyCode::NUMPAD_0));

    return "<no conversion set>";
}

inline std::string to_string(const vrm::MouseCode& code)
{
    switch(code)
    {
    case vrm::MouseCode::Left:
        return "Left mouse button";
        break;
    case vrm::MouseCode::Right:
        return "Right mouse button";
        break;
    case vrm::MouseCode::Middle:
        return "Middle mouse button";
        break;
    case vrm::MouseCode::None:
        return "None";
        break;
    }

    int asInt = static_cast<int>(code);

    if (asInt >= static_cast<int>(vrm::MouseCode::MB4) && asInt <= static_cast<int>(vrm::MouseCode::MB8))
        return std::string("Mouse button ") + static_cast<char>('4' + static_cast<int>(code) - static_cast<int>(vrm::MouseCode::MB4));

    return "<no conversion set>";
}

}