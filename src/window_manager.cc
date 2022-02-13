#include<format>
#include<functional>
#include<iostream>
#include<memory>

#include"common.h"
#include"error_codes.h"
#include"debug_gui_layer.h"
#include"window_manager.h"

enum update_type{
    CAMERA,
    READER
};

void error_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param){
    std::cerr <<
        std::format("ERROR> OpenGL function returned not NO_ERROR(0) error status:\n\tsource: {}\n\ttype: {}\n\tid: {}\n\tseverity: {}\n\t: message: {}\n\tuser param: {}",
                    source, type, id, severity, message, user_param) << std::endl;
}

void error_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param){
#if DEBUG_BUILD
    error_message(source, type, id, severity, length, message, user_param);
#else
    if(type == GL_DEBUG_TYPE_ERROR){
        error_message(source, type, id, severity, length, message, user_param);
    }
#endif
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) noexcept{
    printf("INFO> window resized: (%dx%d).\n", width, height);
    glViewport(0, 0, width, height);
}

int window_manager::init(){
    if(initialized) return NO_ERROR; //NOTE: window is initialized already no more action required

    if(glfwInit() != GLFW_TRUE){
        std::cerr << "ERROR> cannot initialize glfw!" << std::endl;
        return FAILED_TO_INITIALIZE_GLFW;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    //TODO(SyntaX): add proper deleter
    main_window_ptr = {
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.data(), nullptr, nullptr),
        [](GLFWwindow* ptr){         
            glfwDestroyWindow(ptr);
            glfwTerminate(); 
        }
    };

    if(main_window_ptr == nullptr){
        std::cerr << "ERROR> cannot create window!" << std::endl;
        glfwTerminate();
        return FAILED_TO_CREATE_WINDOW;
    }

    glfwSetFramebufferSizeCallback(main_window_ptr.get(), framebuffer_size_callback);
    glfwMakeContextCurrent(main_window_ptr.get());

    glfwSwapInterval(0); //NOTE: just for performance test purpose

    glewExperimental = true;

    if(glewInit() != GLEW_OK){
        std::cerr << "ERROR> cannot initialize glew!" << std::endl;
        glfwTerminate();
        return FAILED_TO_INITIALIZE_GLEW;
    }

    glEnable(GL_DEBUG_OUTPUT);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_SAMPLE_COVERAGE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glDebugMessageCallback(error_message_callback, nullptr);

    user_input_handler_ptr = std::make_unique<user_input_reader>(main_window_ptr);

    if(user_input_handler_ptr == nullptr){
        std::cerr << "ERROR> cannot create user input handler!" << std::endl;
        glfwTerminate();
        return FAILED_TO_CREATE_USER_INPUT_HANDLER;
    }

    application_content_ptr = std::make_unique<content>();

    if(application_content_ptr == nullptr){
        std::cerr << "ERROR> cannot create application content!" << std::endl;
        glfwTerminate();
        return FAILED_TO_CREATE_APPLICATION_CONTENT;
    }

    if(auto error_code = application_content_ptr->init()){
        std::cerr << "ERROR> cannot initialize application content!" << std::endl;
        glfwTerminate();
        return FAILED_TO_INITIALIZE_APPLICATION_CONTENT;
    }

    glfwSetInputMode(main_window_ptr.get(), GLFW_STICKY_KEYS, GL_TRUE);

    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(SKY_COLOR, 1);

    initialized = true;

    return NO_ERROR;
}

void window_manager::input_handler(){
    if(user_input_handler_ptr->is_key_pressed(GLFW_MOUSE_BUTTON_RIGHT, device_type::MOUSE)){
        glm::vec2 cursor_pos = user_input_handler_ptr->get_cursor_position();
        application_content_ptr->get_current_scene()->update(move_camera{cursor_pos});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_W, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{MOVE_FORWARD});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_S, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{MOVE_BACKWARD});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_A, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{STRAFE_LEFT});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_D, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{STRAFE_RIGHT});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_UP, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{MOVE_UP});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_DOWN, device_type::KEYBOARD)){
        application_content_ptr->get_current_scene()->update(move_camera{MOVE_DOWN});
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_PAGE_UP, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_PAGE_UP));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_PAGE_DOWN, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_PAGE_DOWN));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_EQUAL, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_EQUAL));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_MINUS, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_MINUS));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_M, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_M));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_SPACE, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_SPACE));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_COMMA, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_COMMA));
    }
    if(user_input_handler_ptr->is_key_pressed(GLFW_KEY_PERIOD, device_type::KEYBOARD)){
        void(user_input_handler_ptr->register_keystroke(GLFW_KEY_PERIOD));
    }

    switch(user_input_handler_ptr->get_keystroke()){
        case 0: //NOTE: (NO_ACTION) cannot retrieve keystroke with limited stroke rate yet
            break;
        case GLFW_KEY_PAGE_UP:
            application_content_ptr->get_current_scene()->update(scroll_renderer{SCROLL_UP});
            break;
        case GLFW_KEY_PAGE_DOWN:
            application_content_ptr->get_current_scene()->update(scroll_renderer{SCROLL_DOWN});
            break;
        case GLFW_KEY_EQUAL:
            application_content_ptr->volume_up();
            break;
        case GLFW_KEY_MINUS:
            application_content_ptr->volume_down();
            break;
        case GLFW_KEY_M:
            application_content_ptr->volume_mute();
            break;
        case GLFW_KEY_SPACE:
            application_content_ptr->start_stop_playing();
            break;
        case GLFW_KEY_COMMA:
            application_content_ptr->previous_scene();
            break;
        case GLFW_KEY_PERIOD:
            application_content_ptr->next_scene();
            break;
        default: //NOTE: should never happen
            break;
    }
}

void window_manager::handle(){
    if(!initialized){
        std::cout << "INFO> window is not initialized!" << std::endl;
        return;
    }

    application_content_ptr->start_audio_playback();

    double last_timestamp = glfwGetTime();

    unsigned int frame_no = 1;

    GLFWwindow* const raw_main_window_ptr = main_window_ptr.get();

    debug_gui_layer::init(raw_main_window_ptr);

    do{
        if(debug_gui_layer::get_debug_flag()){
            debug_gui_layer::new_frame();
            debug_gui_layer::begin_frame("FPS counter");
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("AppName: Reader");
            ImGui::Text("FPS: %.2f; %.3f ms/frame", io.Framerate, 1000.f / io.Framerate);
        }

        input_handler();

        application_content_ptr->render_scene();
        
        if(debug_gui_layer::get_debug_flag()){
            debug_gui_layer::end_frame();
            debug_gui_layer::render();
        }

        const double current_timestamp = glfwGetTime();
        const double timestamp_diff = current_timestamp - last_timestamp;

        if(timestamp_diff > 1.0){
            std::cout << std::format("INFO> FPS: {}.", frame_no / timestamp_diff) << std::endl;
            frame_no = 0;
            last_timestamp = current_timestamp;
        }

        glfwSwapBuffers(raw_main_window_ptr);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        frame_no++;

    } while(glfwWindowShouldClose(raw_main_window_ptr) == 0);

    debug_gui_layer::destroy();
}
