#pragma once

#include<memory>
#include<string_view>
#include<vector>

#include"audio_playback.h"
//#include"background.h"
#include"background_renderer.h"
#include"basic_object.h"
#include"basic_scene.h"
#include"event.h"
#include"file_reader.h"
#include"shadow_map_maker.h"
#include"text.h"
#include"text_renderer.h"

class text_and_background_scene: public basic_scene{
    file_reader       file_reader_object;
    text              text_object;
    shadow_map_maker  shadow_map_maker_object;
    std::vector<std::shared_ptr<basic_object>> background_objects;
    std::shared_ptr<text_renderer>             text_renderer_ptr;
    std::shared_ptr<background_renderer>       background_renderer_ptr;

public:
    text_and_background_scene(
        std::string_view,
        std::shared_ptr<audio_playback>,
        std::shared_ptr<text_renderer>,
        std::shared_ptr<background_renderer>);

    void add_object(std::shared_ptr<basic_object>) override;
    void build() noexcept override;
    
    void update(const base_event&) noexcept override;

    int is_valid() const override;
};
