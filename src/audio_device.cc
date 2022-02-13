#include<format>
#include<iostream>

#include"audio_device.h"

audio_device::audio_device(){
    device = alcOpenDevice(nullptr); //NOTE: open current device

    if(device == nullptr){
        set_status(expandable_enum::FAILED_TO_OPEN_DEVICE);
        return;
    }

    context = alcCreateContext(device, nullptr);

    if(!alcMakeContextCurrent(context)){
        set_status(expandable_enum::FAILED_TO_CREATE_CONTEXT);
        return;
    }

    if(alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE){
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    } else if(alcGetError(device) == ALC_NO_ERROR){
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    } else{
        set_status(expandable_enum::FAILED_TO_GET_DEVICE_SPECIFIER);
        return;
    }

    std::cout << std::format("INFO> opened audio device info: {}", name) << std::endl;
}

audio_device::~audio_device(){
    buffer.clear();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    if(alcCloseDevice(device) != AL_TRUE){ std::cerr << "ERROR> failed to close device!" << std::endl; return; }
}

int audio_device::add_sound(std::string_view filename){
    return buffer.add_sound(filename);
}

int audio_device::remove_sound(int track_position) noexcept{
    if(track_position < 0 || track_position >= get_buffer_size()) return 0;
    if(track_position < current_track_pos){
        current_track_pos -= 1;
    } else if(track_position == current_track_pos){
        if(previous_exists()){
            current_track_pos -= 1;
        } else if(!next_exists()){ //NOTE: last track in buffer
            current_track_pos = 0;
        }
    }
    buffer.remove_sound(track_position);
}

ALuint audio_device::get_sound(size_t id) const{
    return buffer.get_sound(id);
}

ALuint audio_device::get_current_sound() const{
    return buffer.get_sound(current_track_pos);
}

size_t audio_device::get_buffer_size() const noexcept{
    return buffer.get_size();
}

void audio_device::set_track_id(ALuint new_track_id) noexcept{
    current_track_pos = new_track_id;
}

int audio_device::previous_exists() const noexcept{
    return !(current_track_pos - 1 < 0); //NOTE: current_track_pos is first track
}

//NOTE: sub-expression may overflow before being assigned to a wider type
int audio_device::next_exists() const noexcept{
    return !(current_track_pos + 1 >= buffer.get_size()); //NOTE: current_track_pos is last track
}

void audio_device::set_previous_track() noexcept{
    if(!previous_exists()) return;
    current_track_pos -= 1;
}

void audio_device::set_next_track() noexcept{
    if(!next_exists()) return;
    current_track_pos += 1;
}

void audio_device::set_status(const expandable_enum::audio_device_status_error& new_status) noexcept{
    status = static_cast<int>(new_status);
}

int audio_device::is_valid() const noexcept{
    return status == expandable_enum::NO_ERROR;
}
