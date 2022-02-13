#pragma once

#include<AL\al.h>

#include"audio_reverb.h"
#include"expandable_enum.h"

namespace expandable_enum{
    class audio_source_status_error: public enum_status_error{
        public:
        constexpr audio_source_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
    };

    constexpr audio_source_status_error FAILED_TO_SET_SOURCE_CONFIG = 8192;
}

constexpr float PITCH_HALF = 0.5f;
constexpr float PITCH_NORMAL = 1.0f;
constexpr float PITCH_DOUBLE = 2.0f;
constexpr float MAX_SOURCE_GAIN = 2.0f;

struct source_config{
    ALfloat pitch          = 0.0f;
    ALfloat gain           = 0.0f;
    ALfloat position[3]    = {0.0f, 0.0f, 0.0f};
    ALfloat velocity[3]    = {0.0f, 0.0f, 0.0f};
    ALfloat orientation[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; //TODO(SyntaX): change to default OpenAL value
    ALint   relative       = AL_FALSE;
    ALint   loop           = AL_FALSE;
    ALint   buffer_id      = 0;
};

class audio_source{
    ALuint id = 0;
    source_config config;
    audio_reverb  reverb;

    int status = 0; //NOTE: NO_ERROR

    int set_config(const source_config&);
    void set_status(const expandable_enum::audio_source_status_error&) noexcept;

public:
    audio_source(source_config) noexcept;
    ~audio_source() noexcept;

    int change_config(const source_config&);
    void attach_buffer(ALuint) noexcept;
    void play() const;
    void pause() const;
    void stop() const;
    void mute() noexcept;

    int is_valid() const noexcept;
    constexpr int get_status() const noexcept { return status; }
};
