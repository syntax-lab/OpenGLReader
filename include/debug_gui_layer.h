#pragma once

#ifndef DEBUG_GUI_LAYER_H
#define DEBUG_GUI_LAYER_H
#define DEBUG_ENABLE

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

//TODO(SyntaX): rethink debug_gui_layer design
struct debug_gui_layer{
    static inline int is_debug = true;

    static int init(GLFWwindow* const);
    static void destroy();
    static void new_frame();
    static void begin_frame(const char* const);
    static void end_frame();
    static void render();
    static float get_frame_rate();

    static void set_debug_flag(int) noexcept;
    static int get_debug_flag() noexcept;
};

#endif // DEBUG_GUI_LAYER_H