#pragma once

#ifndef EVENT_H
    #define EVENT_H

    #include<glm/glm.hpp>

    #if 0

    enum struct event_type: unsigned int{
        NO_EVENT,
        KEY_PRESSED,
        MOUSE_MOVEMENT,
        SCROLL_SCREEN
    };

    template<typename... Types> struct args_pack;

    template<typename Type1> struct args_pack<Type1>{
        Type1 arg1;

        constexpr static size_t args_count = 1;

        args_pack(Type1 arg1) noexcept:
            arg1(arg1){}
    };

    template<typename Type1, typename Type2> struct args_pack<Type1, Type2>{
        Type1 arg1;
        Type2 arg2;

        constexpr static size_t args_count = 2;

       args_pack(Type1 arg1, Type2 arg2) noexcept:
            arg1(arg1), arg2(arg2){}
    };

    template<typename Type1, typename Type2, typename Type3> 
    struct args_pack<Type1, Type2, Type3>{
        Type1 arg1;
        Type2 arg2;
        Type3 arg3;

        constexpr static size_t args_count = 3;

        args_pack(Type1 arg1, Type2 arg2, Type3 arg3) noexcept:
            arg1(arg1), arg2(arg2), arg3(arg3){}
    };

    template<typename Type1, typename Type2, typename Type3, typename Type4> 
    struct args_pack<Type1, Type2, Type3, Type4>{
        Type1 arg1;
        Type2 arg2;
        Type3 arg3;
        Type4 arg4;

        constexpr static size_t args_count = 4;

        args_pack(Type1 arg1, Type2 arg2, Type3 arg3, Type4 arg4) noexcept:
            arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4){}
    };

    //TODO(SyntaX): make it dynamic not static
    template<typename... Args>
    struct event{
        event_type type;
        args_pack<Args...> data;

        event(event_type set_event, Args... additional_data) noexcept: type{set_event}, data{additional_data...}{
        
        }
    };

    #endif

    enum event_types{
        BASE_EVENT = -1,
        NO_EVENT,
        KEYBOARD_EVENT,
        MOUSE_EVENT,
        MOVE_CAMERA_EVENT,
        SCROLL_TEXT_EVENT,
        AUDIO_VOLUME_EVENT,
        AUDIO_PLAY_EVENT,
        SCENE_CHANGE_EVENT
    };

    enum event_defines{
        MOVE_FORWARD = 1,
        MOVE_BACKWARD,
        STRAFE_LEFT,
        STRAFE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,

        SCROLL_UP,
        SCROLL_DOWN,

        VOLUME_UP,
        VOLUME_DOWN,
        VOLUME_MUTE,
        AUDIO_START_STOP,
        AUDIO_START,
        AUDIO_STOP,

        SCENE_NEXT,
        SCENE_PREVIOUS
    };

    struct base_event{ 
        static inline int global_id = 0;
                      int        id = 0; 
                      int type      = BASE_EVENT;

        virtual ~base_event(){}
    };

    struct keyboard_event: base_event{
        int button_id;

        keyboard_event(int pressed_button_id): button_id(pressed_button_id) {
            id = ++global_id;
            type = KEYBOARD_EVENT;
        }
    };

    struct mouse_event: base_event{
        int button_id;
        glm::vec2 position; //NOTE: current mouse position

        mouse_event(int pressed_button_id, glm::vec2 new_position): 
            button_id{pressed_button_id},
            position{new_position}{
            id = ++global_id;
            type = MOUSE_EVENT;
        }
    };

    struct move_camera: base_event{
        int direction = -1;
        glm::vec2 position;

        move_camera(int move_direction): 
            direction{move_direction},
            position{}{
            id = ++global_id;
            type = MOVE_CAMERA_EVENT;
        }

        move_camera(glm::vec2 new_position): position{new_position}{
            id = ++global_id;
            type = MOVE_CAMERA_EVENT;
        }
    };

    struct scroll_renderer: base_event{
        int direction;

        scroll_renderer(int scroll_direction): direction{scroll_direction}{
            id = ++global_id;
            type = SCROLL_TEXT_EVENT;
        }
    };

    struct audio_volume: base_event{
        int direction;

        audio_volume(int volume_direction): direction{volume_direction}{
            id = ++global_id;
            type = AUDIO_VOLUME_EVENT;
        }
    };

    struct audio_play: base_event{
        int action;

        audio_play(int new_action): action{new_action}{
            id = ++global_id;
            type = AUDIO_PLAY_EVENT;
        }
    };

    struct scene_change: base_event{
        int direction;

        scene_change(int move_direction): direction{move_direction}{
            id = ++global_id;
            type = SCENE_CHANGE_EVENT;
        }
    };
#endif /* EVENT_H */
