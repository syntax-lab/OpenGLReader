#pragma once

#include<AL\al.h>

#include"expandable_enum.h"

namespace expandable_enum{
    class audio_listener_status_error: public enum_status_error{
        public:
        constexpr audio_listener_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
    };

    constexpr audio_listener_status_error FAILED_TO_SET_LISTENER_CONFIG = 16384;
}

constexpr float MAX_LISTENER_GAIN = 1.25f;
constexpr float GAIN_STEP = 0.125f; //10% of max gain

struct listener_config{
    ALfloat gain           = 0.0f;
    ALfloat position[3]    = {0.0f, 0.0f, 0.0f};
    ALfloat velocity[3]    = {0.0f, 0.0f, 0.0f};
    ALfloat orientation[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
};

class audio_listener{
    listener_config config;

    ALfloat last_gain = MAX_LISTENER_GAIN;
    int muted = 0;

    int status = 0; //NOTE: NO_ERROR

    int set_config(const listener_config&);
    void set_status(const expandable_enum::audio_listener_status_error&) noexcept;
    void update_volume() const;

public:
    audio_listener(listener_config) noexcept;

    int change_config(const listener_config&);
    void unmute();
    void mute();
    void volume_up();
    void volume_down();

    int is_valid() const noexcept;
    constexpr int get_status() const noexcept { return status; }
};
