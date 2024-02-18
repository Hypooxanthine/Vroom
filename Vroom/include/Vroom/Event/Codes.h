#pragma once

namespace vrm
{

enum class KeyCode
{
    None = 0,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Space, Escape, Enter, Tab,
    Right, Left, Down, Up,
    NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
};

enum class MouseCode
{
    None = 0,
    MB1, Left = MB1, MB2, Right = MB2, MB3, Middle = MB3,
    MB4, MB5, MB6, MB7, MB8
};

} // namespace vrm
