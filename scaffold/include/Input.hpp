#pragma once

#include <KeyCodes.h>
#include <unordered_map>

namespace Scaffold
{
    class Input
    {
    public:
        Input(GLFWwindow* window);

        void SetKeyState(KeyCode key, KeyState state);
        void SetMouseButtonState(MouseButton button, KeyState state);

        void UpdateKeyStates();

        KeyState GetKeyState(KeyCode key);
        KeyState GetMouseButtonState(MouseButton button);

        bool IsKeyDown(KeyCode key);
        bool IsMouseButtonDown(MouseButton button);

    private:
        std::unordered_map<KeyCode, KeyState> m_keyMap;
        std::unordered_map<MouseButton, KeyState> m_mouseMap;
    };
}