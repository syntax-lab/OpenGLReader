#pragma once

#include<memory>
#include<vector>

#include<glm/glm.hpp>

#include"background_renderer.h"

class shadow_map_maker{
    unsigned int depth_map_frame_buffer_id;
    unsigned int depth_map;

public:
    shadow_map_maker() noexcept;
    ~shadow_map_maker() noexcept;

    //TODO(SyntaX): should be done in initialization???
    void prepare_depth_map(
        std::shared_ptr<background_renderer>, 
        const std::vector<std::shared_ptr<basic_object>>&,
        const glm::mat4&) const noexcept;

    void apply_depth_map() const noexcept;
    void detach_depth_map() const noexcept;
};
