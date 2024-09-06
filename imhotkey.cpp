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
        std::map<std::string, unsigned short> modNameToFlag{
            {"Shift", ImHotkeyModifier_Shift}, {"Alt", ImHotkeyModifier_Alt},
            {"Ctrl", ImHotkeyModifier_Ctrl},
        };

        std::array<std::string, 5> mouseNames
                {"Mouse Left", "Mouse Right", "Mouse Middle", "Mouse4", "Mouse5"};

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
