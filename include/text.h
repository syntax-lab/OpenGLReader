#pragma once

#include<deque>
#include<string>
#include<string_view>
#include<vector>

#include<glm/glm.hpp>

#include"basic_object.h"
#include"event.h"
#include"lines_buffer.h"

class text: public basic_object{
    lines_buffer lines_buffer_object;

public:
    text(glm::vec3, const std::vector<std::string>&);

    const std::deque<std::string_view>& get_lines_buffer() const noexcept;

    void update() noexcept override;

    void set_event(const base_event&) noexcept override;
};
