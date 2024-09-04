#include "imhotkey.h"

#include <iostream>

namespace ImGui
{
    namespace
    {
        const char* capturing_label = nullptr;

        constexpr ImVec2 DEFAULT_SIZE(60, 30);
    }


    bool ImHotkey(ImHotkeyData_t* v)
    {
        return ImHotkey(v, DEFAULT_SIZE, ImHotkeyFlags_Default);
    }

    bool ImHotkey(ImHotkeyData_t* v, const ImVec2& size, const ImHotkeyFlags flags)
    {

        if (capturing_label == nullptr) {
            // Start capturing if the button is pressed
            if (Button(v->label, size)) {
                capturing_label = v->label;
                std::cout << "Now capturing: " << capturing_label << std::endl;
            }
            return false;
        }

        const bool is_capturing_widget = capturing_label == v->label;


        // Disable every hotkey widget while we are capturing
        BeginDisabled(true);
        Button(is_capturing_widget ? "Listening..." : v->label, size);
        EndDisabled();
        return false;

        return false;
    }
}
