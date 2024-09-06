cmake_minimum_required(VERSION 3.25)

project(imhotkey)

set(CMAKE_CXX_STANDARD 23)

add_executable(imhotkey WIN32
        imhotkey.h
        imhotkey.cpp
        tests/main.cpp
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_impl_dx9.cpp
        imgui/imgui_impl_dx9.h
        imgui/imgui_impl_win32.cpp
        imgui/imgui_impl_win32.h
        imgui/imgui_internal.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/imstb_rectpack.h
        imgui/imgui_widgets.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
)

target_link_libraries(imhotkey PRIVATE d3d9.lib)
include_directories(imgui)