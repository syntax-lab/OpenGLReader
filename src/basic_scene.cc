#include"event.h"
#include"basic_scene.h"

void basic_scene::audio_start(){
    audio_playback_ptr->start();
}

void basic_scene::volume_up() noexcept{
    audio_playback_ptr->set_event(audio_volume{VOLUME_UP});
}

void basic_scene::volume_down() noexcept{
    audio_playback_ptr->set_event(audio_volume{VOLUME_DOWN});
}

void basic_scene::volume_mute() noexcept{
    audio_playback_ptr->set_event(audio_volume{VOLUME_MUTE});
}

void basic_scene::audio_start_stop() noexcept{
    audio_playback_ptr->set_event(audio_play{AUDIO_START_STOP});
}
