#include<algorithm>
#include<format>
#include<iostream>

#include<AL/alext.h>
#include<sndfile.h>

#include"audio_buffer.h"

audio_buffer::~audio_buffer() noexcept{

}

int audio_buffer::add_sound(std::string_view filename){
    SF_INFO file_info;
    SNDFILE* file_handle = sf_open(filename.data(), SFM_READ, &file_info);

    if(file_handle == nullptr){
        std::cerr << "ERROR> cannot open sound file!" << std::endl;
        return false;
    }

    std::cout << std::format("DEBUG> audio file: `{}`; properties: channels: {}; format: {}; frames: {}; sample_rate: {}; sections: {}; seekable: {}",
                             filename, file_info.channels, file_info.format, file_info.frames, file_info.samplerate, file_info.sections, file_info.seekable) << std::endl;

    if(file_info.channels < 1 || file_info.frames > static_cast<sf_count_t>(INT_MAX / sizeof(short) / file_info.channels)){
        std::cerr << "ERROR> bad sample count!" << std::endl;
        sf_close(file_handle);
        return false;
    }

    ALenum sound_format = AL_NONE;

    //NOTE: number of channels
    switch(file_info.channels){
        case 1:
            sound_format = AL_FORMAT_MONO16;
            break;
        case 2:
            sound_format = AL_FORMAT_STEREO16;
            break;
        case 3:
            if(sf_command(file_handle, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){
                sound_format = AL_FORMAT_BFORMAT2D_16;
            }
            break;
        case 4:
            if(sf_command(file_handle, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){
                sound_format = AL_FORMAT_BFORMAT3D_16;
            }
            break;
        default:
            std::cerr << std::format("ERROR> unsupported number of channels: {}!", file_info.channels);
            sf_close(file_handle);
            return false;
    }

    std::unique_ptr mem_buffer = std::make_unique<short[]>(file_info.frames * file_info.channels);

    sf_count_t frames_no = sf_readf_short(file_handle, mem_buffer.get(), file_info.frames);

    if(frames_no < 1){
        std::cerr << "ERROR> failed to read given number of frames!" << std::endl;
        sf_close(file_handle);
        return false;
    }

    ALsizei bytes_no = static_cast<ALsizei>(frames_no * file_info.channels * sizeof(short));

    ALuint buffer_id = 0;

    alGenBuffers(1, &buffer_id);
    alBufferData(buffer_id, sound_format, mem_buffer.get(), bytes_no, file_info.samplerate);

    sf_close(file_handle);

    ALenum error = alGetError();
    if(error != AL_NO_ERROR){
        std::cerr << std::format("ERROR> OpenAL error: {}!", alGetString(error)) << std::endl;
        if(buffer_id || alIsBuffer(buffer_id)){
            alDeleteBuffers(1, &buffer_id);
        }
        return false;
    }

    buffer.push_back(buffer_id);

    return true;
}

int audio_buffer::remove_sound(ALuint desired_id) noexcept{
    buffer.erase(buffer.begin() + desired_id);
}

ALuint audio_buffer::get_sound(size_t desired_id) const{
    if(desired_id >= get_size())
        return 0;

    return buffer.at(desired_id);
}

size_t audio_buffer::get_size() const noexcept{
    return buffer.size();
}

void audio_buffer::clear() noexcept{
    alDeleteBuffers(static_cast<ALsizei>(buffer.size()), buffer.data());
}
