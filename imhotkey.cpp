#include "imhotkey.h"

#include <iostream>

namespace ImGui
{
    namespace
    {
        const char* capturing_label = nullptr;

        constexpr ImVec2 DEFAULT_SIZE(60, 30);
    }


    const char* ImHotkeyData_t::getLabel()
    {
        const int32_t sum = scanCode + vkCode + mouseButton + modifiers;
        if (label_cache_sum != sum) {
            // TODO: Stringify the hotkey
        }

        label_cache_sum = sum;
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
            if (Button(v->getLabel(), size)) {
                capturing_label = v->getLabel();
                std::cout << "Now capturing: " << capturing_label << std::endl;
            }
            return false;
        }

        const bool is_capturing_widget = capturing_label == v->getLabel();


        // Disable every hotkey widget while we are capturing
        BeginDisabled(true);
        Button(is_capturing_widget ? "Listening..." : v->getLabel(), size);
        EndDisabled();
        return false;

        return false;
    }
}
