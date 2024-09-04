#pragma once
#include "imgui.h"

using ImHotkeyFlags = int;

/**
 * @brief Set of flags to control the behavior of an ImHotkey
 */
enum ImHotkeyFlags_
{
    ImHotkeyFlags_None = 0,
    ImHotkeyFlags_NoModifiers = 1,     // Ignore modifiers Mods (shift, alt, ctrl..)
    ImHotkeyFlags_NoKeyboard = 1 << 1, // Ignore keyboard inputs (modifiers not included)
    ImHotkeyFlags_NoMouse = 1 << 2,    // Ignore Mouse inputs, including side buttons

    ImHotkeyFlags_Default = ImHotkeyFlags_None // Default flags to use
};

namespace ImGui
{
    IMGUI_API bool ImHotkey(const char* label, const ImVec2& size, ImHotkeyFlags flags);
}
