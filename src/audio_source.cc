#include<iostream>
#include<format>

#include<AL/efx.h>

#include"audio_source.h"

audio_source::audio_source(source_config desired_config) noexcept: reverb{EFX_REVERB_PRESET_CONCERTHALL} {
    alGenSources(1, &id);
    if(!set_config(desired_config)){
        set_status(expandable_enum::FAILED_TO_SET_SOURCE_CONFIG);
        return;
    }
}

audio_source::~audio_source() noexcept{
    alDeleteSources(1, &id);
}

int audio_source::set_config(const source_config& desired_config){
    config = desired_config;
    
    alSourcef(id, AL_PITCH, config.pitch);
    alSourcef(id, AL_GAIN, config.gain);
    alSourcefv(id, AL_POSITION, config.position);
    alSourcefv(id, AL_VELOCITY, config.velocity);
    alSourcefv(id, AL_ORIENTATION, config.orientation);
    alSourcei(id, AL_SOURCE_RELATIVE, config.relative);
    alSourcei(id, AL_LOOPING, config.loop);
    alSourcei(id, AL_BUFFER, config.buffer_id);
    
    if(reverb.is_feature_supported()){
        std::cout << "INFO> audio reverb feature is supported!" << std::endl;
        int reverb_status = static_cast<int>(reverb.set_effect());
        if(reverb_status){
            std::cerr << std::format("ERROR> failed to set effect\nreason: {}", 
                reverb_status? 
                "failed to create effects ids":
                "failed to setu effects up"
            ) << std::endl;
            return false;
        }
        ALuint slot_id = reverb.get_slot();
        alSource3i(id, AL_AUXILIARY_SEND_FILTER, slot_id, 0, AL_FILTER_NULL);
    } else{
        std::cout << "INFO> audio reverb feature is not supported!" << std::endl;
    }

    ALenum error = alGetError();
    if(error != AL_NO_ERROR){
        std::cerr << std::format("ERROR> failed to initialize audio source\nreason: {}!", alGetString(error)) << std::endl; //TODO(SyntaX): this print not suppose to be here
        return false;
    }

    return true;
}

int audio_source::change_config(const source_config& desired_config){
    config = desired_config;
    return set_config(desired_config);
}

void audio_source::attach_buffer(ALuint buffer_id) noexcept{
    config.buffer_id = buffer_id;
    alSourcei(id, AL_BUFFER, config.buffer_id);
}

void audio_source::play() const{
    alSourcePlay(id);
    std::cout << "INFO> audio source start playing" << std::endl;
}

void audio_source::pause() const{
    ALint state = AL_INITIAL;
    alGetSourcei(id, AL_SOURCE_STATE, &state);

    if(state == AL_PLAYING){
        alSourcePause(id);
        std::cout << "INFO> audio source paused" << std::endl;
        return;
    }else if(state == AL_PAUSED){
        alSourcePlay(id);
        std::cout << "INFO> audio source unpaused" << std::endl;
        return;
    } else{
        std::cout << "INFO> illegal audio source state!" << std::endl;
        return;
    }

}

void audio_source::stop() const{
    alSourceStop(id);
    std::cout << "INFO> audio source stopped!" << std::endl;
}

void audio_source::mute() noexcept{
    config.gain = 0.0f;
}

void audio_source::set_status(const expandable_enum::audio_source_status_error& new_status) noexcept{
    status = static_cast<int>(new_status);
}

int audio_source::is_valid() const noexcept{
    return status == static_cast<int>(expandable_enum::NO_ERROR);
}
