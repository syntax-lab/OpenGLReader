#pragma once

#include<glm/glm.hpp>

#include"event.h"

class camera{
    glm::vec3 position;
    glm::vec3 direction;
    const glm::vec3 up;
    glm::vec2 last_mouse_position;

    void mouse_update(const glm::vec2&) noexcept;
    void move_forward() noexcept;
    void move_backward() noexcept;
    void move_left() noexcept;
    void move_right() noexcept;
    void move_up() noexcept;
    void move_down() noexcept;

public:
    camera(glm::vec3) noexcept;

    void set_event(const move_camera&) noexcept;
    //TODO(SyntaX): camera::set_event should set event/command and camera::update should execute it
    inline void update() noexcept{

    }

    const glm::vec3& get_position() const;
    glm::mat4 get_view() const;
};
