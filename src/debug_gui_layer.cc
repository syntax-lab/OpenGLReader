#include"debug_gui_layer.h"

int debug_gui_layer::init(GLFWwindow* const raw_window_ptr){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(raw_window_ptr, true);
    ImGui_ImplOpenGL3_Init(); // #version 430 core or #version 460 core

    return 0;
}

void debug_gui_layer::destroy(){
    ImGui::DestroyContext();
}

void debug_gui_layer::new_frame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

 float debug_gui_layer::get_frame_rate(){
    ImGui::GetIO().Framerate;
}

void debug_gui_layer::begin_frame(const char* const title){
    ImGui::Begin(title);
}

void debug_gui_layer::end_frame(){
    ImGui::End();
}

void debug_gui_layer::render(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void debug_gui_layer::set_debug_flag(int new_state) noexcept{
    is_debug = new_state;
}

int debug_gui_layer::get_debug_flag() noexcept{
    return is_debug;
}
