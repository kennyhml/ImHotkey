#include "imhotkey.h"
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


namespace ImGui
{
    namespace
    {
        const char* capturing_label = nullptr;

        constexpr ImVec2 DEFAULT_SIZE(60, 30);

        HHOOK gMouseHook = nullptr;
        HHOOK gKeyboardHook = nullptr;

        LRESULT CALLBACK KeyboardEventProc(int nCode, WPARAM wParam, LPARAM lParam);

        LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam);

        DWORD ApplyEventHook(const int id, const HOOKPROC& eventProc, HHOOK& handle)
        {
            handle = SetWindowsHookExA(id, eventProc, GetModuleHandleA(nullptr), 0);

            MSG message;
            while (capturing_label != nullptr) {
                if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
            UnhookWindowsHookEx(handle);
            return 0;
        }
    }


    ImHotkeyData_t::ImHotkeyData_t(const unsigned short t_scanCode,
                                   const unsigned short t_vkCode,
                                   const unsigned short t_mouseButtons,
                                   const unsigned short t_modifiers)
        : scanCode(t_scanCode), vkCode(t_vkCode), mouseButton(t_mouseButtons),
          modifiers(t_modifiers) {}

    const char* ImHotkeyData_t::GetLabel()
    {
        const int32_t sum = scanCode + vkCode + mouseButton + modifiers;
        if (labelCacheSum != sum) {}
        labelCacheSum = sum;
        return label.c_str();
    }

    bool ImHotkey(ImHotkeyData_t* v)
    {
        return ImHotkey(v, DEFAULT_SIZE, ImHotkeyFlags_Default);
    }

    bool ImHotkey(ImHotkeyData_t* v, const ImVec2& size, const ImHotkeyFlags flags)
    {
        if (capturing_label == nullptr) {
            // Start capturing if the button is pressed
            if (Button(v->GetLabel(), size)) {
                capturing_label = v->GetLabel();
            }
            return false;
        }


        const bool is_capturing_widget = capturing_label == v->GetLabel();

        // Disable every hotkey widget while we are capturing
        BeginDisabled(true);
        Button(is_capturing_widget ? "Listening..." : v->GetLabel(), size);
        EndDisabled();
        return false;
    }
}
