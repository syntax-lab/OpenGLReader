#pragma once

#include<memory>
#include<string_view>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"content.h"
#include"user_input_reader.h"

constexpr unsigned int WINDOW_WIDTH  = 1024;
constexpr unsigned int WINDOW_HEIGHT =  768;

class window_manager{
    std::string_view title        = "reader";
    double width                  = 0;
    double height                 = 0;

    std::shared_ptr<GLFWwindow>        main_window_ptr;
    std::unique_ptr<user_input_reader> user_input_handler_ptr;
    std::unique_ptr<content>           application_content_ptr;

    bool initialized = false;

public:

    int init();
    void input_handler();
    void handle();
};
