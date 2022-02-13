#pragma once

#include<array>
#include<memory>
#include<string_view>
#include<vector>

#include"renderer.h"
#include"basic_scene.h"
#include"text_renderer.h"

constexpr unsigned int SCENE_NUMBER = 5;

class content{
    std::shared_ptr<renderer>                              renderer_ptr;
    std::array<std::shared_ptr<basic_scene>, SCENE_NUMBER> scenes_renderer_ptr;

    //NOTE: those scenes are actually acts
    std::vector<std::string_view> scenes_texts{
        "resources/Romeo_and_Juliet_scene_1.txt",
        "resources/Romeo_and_Juliet_scene_2.txt",
        "resources/Romeo_and_Juliet_scene_3.txt",
        "resources/Romeo_and_Juliet_scene_4.txt",
        "resources/Romeo_and_Juliet_scene_5.txt",
    };

    std::vector<std::string_view> scenes_music_themes{
        "resources/01 ArcaniA - Maintheme.wav",
        "resources/22 Xardas Tower.wav",
        "resources/02 Arrival in Feshyr.wav",
        "resources/03 The Hills of Stewark.wav",
        "resources/05 In the Woods.wav"
    };

    unsigned int current_scene_id = 0;

public:
    int init();
    void set_current_scene(unsigned int) noexcept;
    std::shared_ptr<basic_scene>& get_current_scene() noexcept;
    void render_scene() noexcept;
    void previous_scene() noexcept;
    void next_scene() noexcept;

    void start_audio_playback();
    void volume_up() noexcept;
    void volume_down() noexcept;
    void volume_mute() noexcept;
    void start_stop_playing() noexcept;
};
