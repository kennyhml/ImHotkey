#pragma once
#include "imgui.h"

using ImHotkeyFlags = int;

/**
 * @brief Set of flags to control the behavior of an ImHotkey
 */
enum ImHotkeyFlags_
{
    ImHotkeyFlags_None = 0,
    ImHotkeyFlags_NoKeyboard = 1,
    ImHotkeyFlags_NoMouse = 1 << 1,

    ImHotkeyFlags_Default = ImHotkeyFlags_None
};

namespace ImGui
{
    IMGUI_API bool ImHotkey(const char* label, const ImVec2& size, ImHotkeyFlags flags);
}
