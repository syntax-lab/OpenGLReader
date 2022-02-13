#pragma once

#include<string_view>

#include<AL/al.h>
#include<AL/alc.h>

#include"audio_buffer.h"
#include"expandable_enum.h"

namespace expandable_enum{
    class audio_device_status_error: public enum_status_error{
    public:
        constexpr audio_device_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
    };

    constexpr audio_device_status_error FAILED_TO_OPEN_DEVICE = 1024;
    constexpr audio_device_status_error FAILED_TO_CREATE_CONTEXT = 2048;
    constexpr audio_device_status_error FAILED_TO_GET_DEVICE_SPECIFIER = 4096;
}

class audio_device{
    ALCdevice*    device  = nullptr;
    ALCcontext*   context = nullptr;
    const ALchar* name    = nullptr;
    int           status  = 0; //NOTE: NO_ERROR
    audio_buffer  buffer;
    
    int current_track_pos = 0;

    void set_status(const expandable_enum::audio_device_status_error&) noexcept;

public:
    audio_device();
    ~audio_device();

    int add_sound(std::string_view);
    int remove_sound(int) noexcept;
    ALuint get_sound(size_t) const;
    ALuint get_current_sound() const;
    size_t get_buffer_size() const noexcept;
    void set_track_id(ALuint) noexcept;
    int previous_exists() const noexcept;
    int next_exists() const noexcept;
    void set_previous_track() noexcept;
    void set_next_track() noexcept;

    int is_valid() const noexcept;
    constexpr int get_status() const noexcept { return status; }
};
