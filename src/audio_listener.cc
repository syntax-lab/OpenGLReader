#include<format> 
#include<iostream>

#include"audio_listener.h"

audio_listener::audio_listener(listener_config desired_config) noexcept{
    if(!set_config(desired_config)){
        set_status(expandable_enum::FAILED_TO_SET_LISTENER_CONFIG);
        return;
    }
}

int audio_listener::set_config(const listener_config& desired_config){
    config = desired_config;

    alListenerf(AL_GAIN, config.gain);
    alListenerfv(AL_POSITION, config.position);
    alListenerfv(AL_VELOCITY, config.velocity);
    alListenerfv(AL_ORIENTATION, config.orientation);
    
    ALenum error = alGetError();
    if(error != AL_NO_ERROR){
        std::cerr << std::format("ERROR> cannot initialize listener, reason: {}", alGetString(error)) << std::endl;
        return false;
    }

    return true;
}

int audio_listener::change_config(const listener_config& desired_config){
    if(!set_config(desired_config))
        return false;
    config = desired_config;
    return true;
}

void audio_listener::update_volume() const{
    alListenerf(AL_GAIN, config.gain);
    std::cout << std::format("INFO> gain: {}", config.gain) << std::endl;
}

void audio_listener::unmute(){
    if(muted){
        config.gain = last_gain;
        muted = 0; //NOTE: false
        std::cout << "INFO> sound unmuted" << std::endl;
    }
}

void audio_listener::mute(){
    if(!muted){
        last_gain = config.gain;
        config.gain = 0.0f;
        muted = 1; //NOTE: true
        std::cout << "INFO> sound muted" << std::endl;
    } else{
        unmute();
    }
    update_volume();
}

void audio_listener::volume_up(){
    unmute();
    config.gain += GAIN_STEP;
    if(config.gain > MAX_LISTENER_GAIN) config.gain = MAX_LISTENER_GAIN;
    update_volume();
}

void audio_listener::volume_down(){
    unmute();
    config.gain -= GAIN_STEP;
    if(config.gain < 0.0f) config.gain = 0.0f;
    update_volume();
}

void audio_listener::set_status(const expandable_enum::audio_listener_status_error& new_status) noexcept{
    status = static_cast<int>(new_status);
}

int audio_listener::is_valid() const noexcept{
    return status == static_cast<int>(expandable_enum::NO_ERROR);
}
