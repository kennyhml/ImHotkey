# ImHotkey
Compact ImGui widget to implement hotkey assignment via a widget.
## Installation
Drop `ImHotkey.h` and `ImHotkey.cpp` into the folder containing your ImGui setup. *The project does not ship with those files!*

**WARNING:** You only want to copy the two files mentioned, the rest of the projects files are purely for testing and development purposes.
## The concept
The underlying widget is an `ImGui::Button` object that can be clicked to activate the input capture threads. 
The captured strokes (received by hooking with `SetWindowsHookExA`), including modifiers, are then displayed as text form on the button and updated in a struct.
