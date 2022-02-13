#pragma once

#include<AL/al.h>
#include<AL/efx-presets.h>

enum class audio_reverb_errors{
    NO_ERROR = 0,
    FAILED_TO_CREATE_EFFECTS_IDS = 1,
    FAILED_TO_SET_EFFECTS = 2
};

class audio_reverb{
    EFXEAXREVERBPROPERTIES active_reverb;
    
    ALuint effect_id = 0;
    ALuint slot_id = 0;

public:
    audio_reverb(const EFXEAXREVERBPROPERTIES&) noexcept;
    ~audio_reverb() noexcept;

    int is_feature_supported() const noexcept;
    audio_reverb_errors set_effect();
    ALuint get_slot() const noexcept;
};
