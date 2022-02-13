#pragma once
#include<bitset>
#include<memory>
#include<queue>
#include<unordered_map>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

enum class keystroke_type: unsigned int{
    SCROLL_PAGE_UP  ,
    SCROLL_PAGE_DOWN,
    SCROLL_LINE_UP  ,
    SCROLL_LINE_DOWN,

    AUDIO_VOLUME_UP,
    AUDIO_VOLUME_DOWN,
    AUDIO_VOLUME_MUTE,
    AUDIO_PLAY,
    AUDIO_STOP,

    SCENE_PREVIOUS,
    SCENE_NEXT
};

constexpr size_t key_stroke_type_size = 11; //NOTE: remember to update this value each time when new keystroke_type added

enum class device_type: unsigned int{
    KEYBOARD,
    MOUSE
};

class user_input_reader{
    //keystroke_id, last_time_stroke
    std::queue<std::tuple<int, double>>     user_input_queue;
    std::bitset<key_stroke_type_size>       keystrokes_in_queue;
    std::unordered_map<int, keystroke_type> keystroke_translation_table;
    std::shared_ptr<GLFWwindow>             window_ptr;

public:
    user_input_reader(std::shared_ptr<GLFWwindow> window_ptr);

    int rebind_key(keystroke_type, int, int);
    keystroke_type translate_keystroke(int) const;
    int is_key_pressed(int, device_type) const noexcept;
    int register_keystroke(int);
    glm::vec2 get_cursor_position() const noexcept;
    int get_keystroke() noexcept; 
    void clear() noexcept;
};
