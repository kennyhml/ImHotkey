#include "imhotkey.h"
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <array>
#include <map>
#include <windows.h>


namespace ImGui
{
    namespace
    {
        // Hold the widget data that is currently being captured in a shared pointer, that
        // way we can ensure the container wont get deleted while the capturing threads
        // are still running and accessing it. Weak pointers would expose us to racing
        ImHotkeyData_t* capturing = nullptr;

        // Default size of an ImHotkey widget
        constexpr ImVec2 DEFAULT_SIZE(60, 30);

        HHOOK gMouseHook = nullptr;
        HHOOK gKeyboardHook = nullptr;

        std::map<std::string, unsigned short> modNameToFlag{
            {"Shift", ImHotkeyModifier_Shift}, {"Alt", ImHotkeyModifier_Alt},
            {"Ctrl", ImHotkeyModifier_Ctrl},
        };

        std::array<std::string, 5> mouseNames
                {"Mouse Left", "Mouse Right", "Mouse Middle", "Mouse4", "Mouse5"};

        LRESULT CALLBACK KeyboardEventProc(const int nCode, const WPARAM wParam,
                                           const LPARAM lParam)
        {
            return CallNextHookEx(gKeyboardHook, nCode, wParam, lParam);
        }

        LRESULT CALLBACK MouseEventProc(const int nCode, const WPARAM wParam,
                                        const LPARAM lParam)
        {
            if (wParam == WM_LBUTTONDOWN) {
                capturing->mouseButton = 1;
            } else if (wParam == WM_RBUTTONDOWN) {
                capturing->mouseButton = 2;
            } else if (wParam == WM_MBUTTONDOWN) {
                capturing->mouseButton = 3;
            } else if (wParam == WM_XBUTTONDOWN) {
                const MSLLHOOKSTRUCT* event = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
                capturing->mouseButton = event->mouseData == 65536 ? 4 : 5;
            }

            return CallNextHookEx(gMouseHook, nCode, wParam, lParam);
        }

        DWORD ApplyEventHook(const int id, const HOOKPROC& eventProc, HHOOK& handle)
        {
            handle = SetWindowsHookExA(id, eventProc, GetModuleHandleA(nullptr), 0);

            std::thread([handle] {
                MSG message;
                while (capturing) {
                    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
                        TranslateMessage(&message);
                        DispatchMessage(&message);
                    }
                }
                UnhookWindowsHookEx(handle);
            }).detach();
            return 0;
        }
    }

    ImHotkeyData_t::ImHotkeyData_t(const unsigned short t_scanCode,
                                   const unsigned short t_vkCode,
                                   const unsigned short t_mouseButtons,
                                   const unsigned short t_modifiers)
        : scanCode(t_scanCode), vkCode(t_vkCode), mouseButton(t_mouseButtons),
          modifiers(t_modifiers), id_(instanceCount++) {}

    const char* ImHotkeyData_t::GetLabel()
    {
        const int32_t sum = scanCode + vkCode + mouseButton + modifiers;
        if (labelCacheSum_ != sum) {
            std::string new_;
            // track how many things we added to determine if we must place " + " first.
            int32_t additions = 0;

            // Place the mods first since the standart is Ctrl + Z rather than Z + Ctrl
            for (const auto& [name, flag]: modNameToFlag) {
                if (modifiers & flag) {
                    new_ += additions++ ? " + " + name : name;
                }
            }

            // There really is no "right" way to place mouse or keys first, either way
            // looks strange and you have to be a psycho to set a hotkey like that anyway.
            if (mouseButton) {
                const std::string name = mouseNames[mouseButton];
                new_ += additions++ ? " + " + name : name;
            }

            if (vkCode) {
                const std::string character(1, static_cast<char>(vkCode));
                new_ += additions++ ? " + " + character : character;
            }
            label_ = new_ + std::format("##{}", id_);
        }
        labelCacheSum_ = sum;
        return label_.c_str();
    }

    bool ImHotkey(ImHotkeyData_t* v)
    {
        return ImHotkey(v, DEFAULT_SIZE, ImHotkeyFlags_Default);
    }

    bool ImHotkey(ImHotkeyData_t* v, const ImVec2& size, const ImHotkeyFlags flags)
    {
        if (!capturing) {
            // Start capturing if the button is pressed
            if (Button(v->GetLabel(), size)) {
                capturing = v;
                if ((flags & ImHotkeyFlags_NoKeyboard) == 0) {
                    ApplyEventHook(WH_KEYBOARD_LL, KeyboardEventProc, gKeyboardHook);
                }
                if ((flags & ImHotkeyFlags_NoMouse) == 0) {
                    ApplyEventHook(WH_MOUSE_LL, MouseEventProc, gMouseHook);
                }
            }
            return false;
        }

        const bool is_active_widget = capturing && v == capturing;

        // Disable every hotkey widget while we are capturing
        BeginDisabled(true);
        Button(is_active_widget ? "Listening..." : v->GetLabel(), size);
        EndDisabled();
        return false;
    }
}
