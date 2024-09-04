#pragma once
#include <cstdint>
#include <string>

#include "imgui.h"

using ImHotkeyFlags = int;

/**
 * @brief Set of flags to control the behavior of an ImHotkey
 */
enum ImHotkeyFlags_
{
    ImHotkeyFlags_None = 0,
    ImHotkeyFlags_NoModifiers = 1,       // Ignore modifiers keys (shift, alt, ctrl..)
    ImHotkeyFlags_NoKeyboard = 1 << 1,   // Ignore the keyboard (modifiers not included)
    ImHotkeyFlags_NoMouse = 1 << 2,      // Ignore the mouse (including side buttons)
    ImHotkeyFlags_OneModifier = 1 << 3,  // Allow a maximum of one modifier key

    ImHotkeyFlags_Default = ImHotkeyFlags_None // Default flags to use
};

namespace ImGui
{
    /**
     * @brief Container for the relevant data of a hotkey.
     */
    struct ImHotkeyData_t
    {
        // The scan code of the key, scan codes are keyboard hardware dependent
        unsigned short scanCode = 0;

        // The virtual keycode of the the key, these are constants.
        unsigned short vkCode = 0;

        // Either left, right, middle, mouse4 or mouse5 (0 - 5)
        unsigned short mouseButton = 0;

        // Either shift, ctrl or alt (0x1, 0x10 or 0x100)
        unsigned short modifiers = 0;

        // The label of the widget is made up from the buttons and modifiers, for example
        // "Alt + F4" or "Ctrl + Z" or "A". Be mindful of label collisions.
        const char* label = nullptr;
    };

    IMGUI_API bool ImHotkey(ImHotkeyData_t* v);

    IMGUI_API bool ImHotkey(ImHotkeyData_t* v, const ImVec2& size);

    IMGUI_API bool ImHotkey(ImHotkeyData_t* v, const ImVec2& size, ImHotkeyFlags flags);
}
