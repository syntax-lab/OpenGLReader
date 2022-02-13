#pragma once

#include<string_view>
#include<vector>

#include"audio_buffer.h"
#include"audio_device.h"
#include"audio_listener.h"
#include"audio_source.h"
#include"event.h"

#include"expandable_enum.h"

namespace expandable_enum{
    class audio_playback_status_error: public enum_status_error{
    public:
        constexpr audio_playback_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
    };

    constexpr audio_playback_status_error FAILED_TO_CREATE_DEVICE = 32768;
    constexpr audio_playback_status_error FAILED_TO_CREATE_SOURCE = 65536;
    constexpr audio_playback_status_error FAILED_TO_CREATE_LISTENER = 131072;
    constexpr audio_playback_status_error FAILED_TO_ADD_BACKGROUND_SOUND = 262144;
}

//TODO(SyntaX): add support for spatial sound
class audio_playback{
    audio_device   device;
    audio_source   source;
    audio_listener listener;

    int status = 0; //NOTE: NO_ERROR

    void volume_up();
    void volume_down();
    void volume_mute();
    void play() const;
    void pause() const;
    void stop() const;

    void set_status(const expandable_enum::audio_playback_status_error&) noexcept;

public:
    audio_playback(const std::vector<std::string_view>&);

    void set_event(const base_event&) noexcept;
    void start() const;
    void change_track(int);
    //TODO(SyntaX): audio_playback::set_event should set event/command and audio_playback::update should execute it
    inline void update() noexcept{
        
    };

    int is_valid() const noexcept;
    constexpr int get_status() const noexcept { return status; }
};
