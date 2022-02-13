#include<iostream>

#include"actions.h"
#include"audio_playback.h"

//TODO(SyntaX): should we move source to device?
audio_playback::audio_playback(const std::vector<std::string_view>& music_themes):
    source{source_config{.pitch = PITCH_NORMAL, .gain = MAX_SOURCE_GAIN, .loop = AL_TRUE}}, 
    listener{listener_config{.gain = MAX_LISTENER_GAIN}}{
    for(auto&& file_path: music_themes){
        if(!device.add_sound(file_path)){
            set_status(expandable_enum::FAILED_TO_ADD_BACKGROUND_SOUND);
            return;
        }
    }

    source.attach_buffer(device.get_sound(0));
}

void audio_playback::volume_up(){
    listener.volume_up();
}

void audio_playback::volume_down(){
    listener.volume_down();
}

void audio_playback::volume_mute(){
    listener.mute();
}

void audio_playback::play() const{
    source.play();
}

void audio_playback::pause() const{
    source.pause();
}

void audio_playback::stop() const{
    source.stop();
}

void audio_playback::start() const{
    play();
}


void audio_playback::change_track(int direction){
        stop();
        if(direction < 0) {
            device.set_previous_track();
        } else{
            device.set_next_track();
        }
        source.attach_buffer(device.get_current_sound());
        //TODO(SyntaX): we should let reverb sound until next track starts play
        play();
}

void audio_playback::set_event(const base_event& event) noexcept{
    using enum action_types;
    switch(event.type){
        case AUDIO_PLAY_EVENT:{
            const audio_play& casted_event = dynamic_cast<const audio_play&>(event);
            switch(casted_event.action){
                case AUDIO_START:
                    play();
                    break;
                case AUDIO_START_STOP:
                    pause();
                    break;
                case AUDIO_STOP:
                    stop();
                    break;
                default:
                    return; //NOTE: should never happend
            }
            break;
        }
        
        case AUDIO_VOLUME_EVENT:{
            const audio_volume& casted_event = dynamic_cast<const audio_volume&>(event);
            switch(casted_event.direction){
                case VOLUME_UP:
                    volume_up();
                    break;
                case VOLUME_DOWN:
                    volume_down();
                    break;
                case VOLUME_MUTE:
                    volume_mute();
                    break;
                default:
                    return; //NOTE: should never happend
            }
            break;
        }
        
        case SCENE_CHANGE_EVENT:{
            const scene_change& casted_event = dynamic_cast<const scene_change&>(event);
            switch(casted_event.direction){
                case SCENE_PREVIOUS:
                    change_track(static_cast<int>(PREVIOUS_ITEM));
                    break;
                case SCENE_NEXT:
                    change_track(static_cast<int>(NEXT_ITEM));
                    break;
                default:
                    return; //NOTE: should never happend
            }
            break;
        }
        default: 
            return; //NOTE: should never happend
    }
}

void audio_playback::set_status(const expandable_enum::audio_playback_status_error& new_status) noexcept{
    status = static_cast<int>(new_status);
}

int audio_playback::is_valid() const noexcept{
    int error_status = status;
    if(device.is_valid())   error_status |= device.get_status();
    if(source.is_valid())   error_status |= source.get_status();
    if(listener.is_valid()) error_status |= listener.get_status();
    return error_status;
}
