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
    ImHotkeyFlags_NoDuplicates = 1 << 3, // The hotkeys must be unique across widgets
    ImHotkeyFlags_OneModifier = 1 << 4,  // Allow a maximum of one modifier key

    ImHotkeyFlags_Default = ImHotkeyFlags_None // Default flags to use
};

namespace ImGui
{
    /**
     * @brief Container for the relevant data of a hotkey.
     */
    struct ImHotkeyData_t
    {
        unsigned short scancode;
        unsigned short vkCode;
        unsigned short modifiers;

        /**
         * @brief Turn the hotkey into a human readable format.
         *
         * Examples: `Alt + F4` | `A` | `Ctrl + Z`
         */
        [[nodiscard]] const std::string& to_string();
    };

    IMGUI_API bool ImHotkey(const char* label, ImHotkeyData_t* v);

    IMGUI_API bool ImHotkey(const char* label, ImHotkeyData_t* v, const ImVec2& size);

    IMGUI_API bool ImHotkey(const char* label, ImHotkeyData_t* v, const ImVec2& size,
                            ImHotkeyFlags flags);
}
