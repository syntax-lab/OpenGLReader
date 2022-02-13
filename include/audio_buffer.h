#pragma once

#include<string_view>
#include<vector>

#include<AL/al.h>

class audio_buffer{
    std::vector<ALuint> buffer;

public:
    ~audio_buffer() noexcept;

    int add_sound(std::string_view);
    int remove_sound(ALuint) noexcept;
    ALuint get_sound(size_t) const;
    size_t get_size() const noexcept;
    void clear() noexcept;
};
