#include<iostream>
#include<format>

#include<AL/efx.h>

#include"audio_reverb.h"

LPALGENEFFECTS alGenEffects                                 = nullptr;
LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots       = nullptr;
LPALEFFECTI alEffecti                                       = nullptr;
LPALEFFECTF alEffectf                                       = nullptr;
LPALEFFECTFV alEffectfv                                     = nullptr;
LPALISEFFECT alIsEffect                                     = nullptr;
LPALDELETEEFFECTS alDeleteEffects                           = nullptr;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti             = nullptr;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = nullptr;

#define LOAD_PROC(type, func_name)  ((func_name) = (type)alGetProcAddress(#func_name))

audio_reverb::audio_reverb(const EFXEAXREVERBPROPERTIES& new_active_reverb) noexcept: active_reverb{new_active_reverb}{

}

audio_reverb::~audio_reverb() noexcept{
    if(alIsEffect(effect_id))
        alDeleteEffects(1, &effect_id);
    alDeleteAuxiliaryEffectSlots(1, &slot_id);
}

int audio_reverb::is_feature_supported() const noexcept{
    auto device = alcGetContextsDevice(alcGetCurrentContext());

    if(!alcIsExtensionPresent(device, "ALC_EXT_EFX")){
        return 0; //NOTE: false
    }

    return 1; //NOTE: true
}

audio_reverb_errors audio_reverb::set_effect(){
    using enum audio_reverb_errors;

    LOAD_PROC(LPALEFFECTI, alEffecti);
    LOAD_PROC(LPALEFFECTF, alEffectf);
    LOAD_PROC(LPALEFFECTFV, alEffectfv);
    LOAD_PROC(LPALGENAUXILIARYEFFECTSLOTS, alGenAuxiliaryEffectSlots);
    LOAD_PROC(LPALAUXILIARYEFFECTSLOTI, alAuxiliaryEffectSloti);
    LOAD_PROC(LPALGENEFFECTS, alGenEffects);
    LOAD_PROC(LPALISEFFECT, alIsEffect);
    LOAD_PROC(LPALDELETEEFFECTS, alDeleteEffects);
    LOAD_PROC(LPALDELETEAUXILIARYEFFECTSLOTS, alDeleteAuxiliaryEffectSlots);

    alGenEffects(1, &effect_id);
    alGenAuxiliaryEffectSlots(1, &slot_id);

    if(alGetError() != AL_NO_ERROR){
        return FAILED_TO_CREATE_EFFECTS_IDS; //NOTE: false
    }

    if(alGetEnumValue("AL_EFFECT_EAXREVERB") != 0){
        std::cout << "INFO> EAX reverb supported!" << std::endl;

        alEffecti(effect_id, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

        alEffectf(effect_id, AL_EAXREVERB_DENSITY, active_reverb.flDensity);
        alEffectf(effect_id, AL_EAXREVERB_DIFFUSION, active_reverb.flDiffusion);
        alEffectf(effect_id, AL_EAXREVERB_GAIN, active_reverb.flGain);
        alEffectf(effect_id, AL_EAXREVERB_GAINHF, active_reverb.flGainHF);
        alEffectf(effect_id, AL_EAXREVERB_GAINLF, active_reverb.flGainLF);
        alEffectf(effect_id, AL_EAXREVERB_DECAY_TIME, active_reverb.flDecayTime);
        alEffectf(effect_id, AL_EAXREVERB_DECAY_HFRATIO, active_reverb.flDecayHFRatio);
        alEffectf(effect_id, AL_EAXREVERB_DECAY_LFRATIO, active_reverb.flDecayLFRatio);
        alEffectf(effect_id, AL_EAXREVERB_REFLECTIONS_GAIN, active_reverb.flReflectionsGain);
        alEffectf(effect_id, AL_EAXREVERB_REFLECTIONS_DELAY, active_reverb.flReflectionsDelay);
        alEffectfv(effect_id, AL_EAXREVERB_REFLECTIONS_PAN, active_reverb.flReflectionsPan);
        alEffectf(effect_id, AL_EAXREVERB_LATE_REVERB_GAIN, active_reverb.flLateReverbGain);
        alEffectf(effect_id, AL_EAXREVERB_LATE_REVERB_DELAY, active_reverb.flLateReverbDelay);
        alEffectfv(effect_id, AL_EAXREVERB_LATE_REVERB_PAN, active_reverb.flLateReverbPan);
        alEffectf(effect_id, AL_EAXREVERB_ECHO_TIME, active_reverb.flEchoTime);
        alEffectf(effect_id, AL_EAXREVERB_ECHO_DEPTH, active_reverb.flEchoDepth);
        alEffectf(effect_id, AL_EAXREVERB_MODULATION_TIME, active_reverb.flModulationTime);
        alEffectf(effect_id, AL_EAXREVERB_MODULATION_DEPTH, active_reverb.flModulationDepth);
        alEffectf(effect_id, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, active_reverb.flAirAbsorptionGainHF);
        alEffectf(effect_id, AL_EAXREVERB_HFREFERENCE, active_reverb.flHFReference);
        alEffectf(effect_id, AL_EAXREVERB_LFREFERENCE, active_reverb.flLFReference);
        alEffectf(effect_id, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, active_reverb.flRoomRolloffFactor);
        alEffecti(effect_id, AL_EAXREVERB_DECAY_HFLIMIT, active_reverb.iDecayHFLimit);

    } else{
        std::cout << "INFO> standard reverb supported!" << std::endl;

        alEffecti(effect_id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

        alEffectf(effect_id, AL_REVERB_DENSITY, active_reverb.flDensity);
        alEffectf(effect_id, AL_REVERB_DIFFUSION, active_reverb.flDiffusion);
        alEffectf(effect_id, AL_REVERB_GAIN, active_reverb.flGain);
        alEffectf(effect_id, AL_REVERB_GAINHF, active_reverb.flGainHF);
        alEffectf(effect_id, AL_REVERB_DECAY_TIME, active_reverb.flDecayTime);
        alEffectf(effect_id, AL_REVERB_DECAY_HFRATIO, active_reverb.flDecayHFRatio);
        alEffectf(effect_id, AL_REVERB_REFLECTIONS_GAIN, active_reverb.flReflectionsGain);
        alEffectf(effect_id, AL_REVERB_REFLECTIONS_DELAY, active_reverb.flReflectionsDelay);
        alEffectf(effect_id, AL_REVERB_LATE_REVERB_GAIN, active_reverb.flLateReverbGain);
        alEffectf(effect_id, AL_REVERB_LATE_REVERB_DELAY, active_reverb.flLateReverbDelay);
        alEffectf(effect_id, AL_REVERB_AIR_ABSORPTION_GAINHF, active_reverb.flAirAbsorptionGainHF);
        alEffectf(effect_id, AL_REVERB_ROOM_ROLLOFF_FACTOR, active_reverb.flRoomRolloffFactor);
        alEffecti(effect_id, AL_REVERB_DECAY_HFLIMIT, active_reverb.iDecayHFLimit);
    }

    alAuxiliaryEffectSloti(slot_id, AL_EFFECTSLOT_EFFECT, effect_id);

    ALenum error = alGetError();
    if(error != AL_NO_ERROR){
        std::cerr << std::format("ERROR> cannot create effect: {}", alGetString(error)) << std::endl;

        if(alIsEffect(effect_id))
            alDeleteEffects(1, &effect_id);

        return FAILED_TO_SET_EFFECTS;
    }

    return NO_ERROR;
}

ALuint audio_reverb::get_slot() const noexcept{
    return slot_id;
}
