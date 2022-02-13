#include"user_input_reader.h"

constexpr double       UPDATE_RATE_THRESHOLD  = 0.1;
constexpr double       STALE_INPUT_THREASHOLD = 2.0;
constexpr unsigned int MAX_QUEUE_SIZE         = 30;

user_input_reader::user_input_reader(std::shared_ptr<GLFWwindow> user_window_ptr): window_ptr{user_window_ptr}{
    keystroke_translation_table[GLFW_KEY_PAGE_UP]   = keystroke_type::SCROLL_PAGE_UP;
    keystroke_translation_table[GLFW_KEY_PAGE_DOWN] = keystroke_type::SCROLL_PAGE_DOWN;
    keystroke_translation_table[GLFW_KEY_EQUAL]     = keystroke_type::AUDIO_VOLUME_UP;
    keystroke_translation_table[GLFW_KEY_MINUS]     = keystroke_type::AUDIO_VOLUME_DOWN;
    keystroke_translation_table[GLFW_KEY_M]         = keystroke_type::AUDIO_VOLUME_MUTE;
    keystroke_translation_table[GLFW_KEY_SPACE]     = keystroke_type::AUDIO_PLAY; 
    keystroke_translation_table[GLFW_KEY_SPACE]     = keystroke_type::AUDIO_STOP; //NOTE: we rebind GLFW_KEY_SPACE and handle it as START/STOP key
    keystroke_translation_table[GLFW_KEY_COMMA]     = keystroke_type::SCENE_PREVIOUS;
    keystroke_translation_table[GLFW_KEY_PERIOD]    = keystroke_type::SCENE_NEXT;
}

int user_input_reader::rebind_key(keystroke_type type, int old_key_id, int new_key_id){
    if(keystroke_translation_table.contains(old_key_id)){
        keystroke_translation_table.erase(old_key_id);
    }
    keystroke_translation_table[new_key_id] = type;
}

keystroke_type user_input_reader::translate_keystroke(int keystroke) const{
    return keystroke_translation_table.at(keystroke);
}

int user_input_reader::is_key_pressed(int keystroke, device_type type) const noexcept{
    switch(type){
        case device_type::KEYBOARD:
            return glfwGetKey(window_ptr.get(), keystroke) == GLFW_PRESS;
        case device_type::MOUSE:
            return glfwGetMouseButton(window_ptr.get(), keystroke) == GLFW_PRESS;
        default:
            return 0; //NOTE: should never happend
    }
}

int user_input_reader::register_keystroke(int keystroke){
    if(user_input_queue.size() >= MAX_QUEUE_SIZE) 
        return 0; //NOTE: false

    double time_delta = 0;
    double current_time = glfwGetTime();
    
    keystroke_type type = translate_keystroke(keystroke);

    if(!user_input_queue.empty()){
        if(keystrokes_in_queue[static_cast<unsigned int>(type)])
            return 0; //NOTE: cannot add more than one event of the same type
        
        auto&& [_, front_time_delta] = user_input_queue.front();
        
        time_delta = current_time - front_time_delta;

        if(time_delta > STALE_INPUT_THREASHOLD){ //NOTE: stale input should be cleared
            clear();
        }

    }
    
    user_input_queue.push({keystroke, current_time});

    keystrokes_in_queue[static_cast<unsigned int>(type)] = 1; //NOTE: mark keystroke as present in queue

    return 1; //NOTE: true
}

glm::vec2 user_input_reader::get_cursor_position() const noexcept{
    double x, y;
    glfwGetCursorPos(window_ptr.get(), &x, &y);
    return {x, y};
}

int user_input_reader::get_keystroke() noexcept{
    if(user_input_queue.empty())
        return 0; //NOTE: false

    double current_time = glfwGetTime();
    double time_delta = current_time - std::get<1>(user_input_queue.front());

    if(time_delta > STALE_INPUT_THREASHOLD){ //NOTE: stale input should be cleared
        clear();
        return 0;
    }

    if(time_delta < UPDATE_RATE_THRESHOLD)
        return 0;  //NOTE: too soon to retrieve event

    int key_stroke = std::get<0>(user_input_queue.front());
    user_input_queue.pop();

    keystrokes_in_queue[static_cast<unsigned int>(translate_keystroke(key_stroke))] = 0; //NOTE: mark keystroke as absent in queue

    return key_stroke;
}

void user_input_reader::clear() noexcept{
    std::queue<std::tuple<int, double>> empty_queue;
    user_input_queue.swap(empty_queue);
    keystrokes_in_queue.reset();
}
