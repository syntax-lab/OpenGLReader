#include<glm/gtx/transform.hpp>

#include "camera.h"

camera::camera(glm::vec3 position) noexcept:
    position{position}, direction{0.f, 0.f, -1.f}, up{0.f, 1.f, 0.f}, last_mouse_position{0.f, 0.f}{}

//TODO(SyntaX): check if static here is better
void camera::mouse_update(const glm::vec2& current_mouse_position) noexcept{
    glm::vec2 delta = current_mouse_position - last_mouse_position;
    if(glm::length(delta) > 50.f) {
        last_mouse_position = current_mouse_position;
        return;
    }
    glm::vec3 cross_product = glm::cross(direction, up);
    glm::mat4 rotate = glm::rotate(delta.x * 0.003f, up) * glm::rotate(delta.y * 0.003f, cross_product);
    direction = glm::mat3(rotate) * direction;
    last_mouse_position = current_mouse_position;
}

void camera::move_forward() noexcept{
    position += direction * 0.1f;
}

void camera::move_backward() noexcept{
    position -= direction * 0.1f;
}

void camera::move_left() noexcept{
    glm::vec3 cross_product = glm::cross(direction, up);
    position -= cross_product * 0.1f;
}

void camera::move_right() noexcept{
    glm::vec3 cross_product = glm::cross(direction, up);
    position += cross_product * 0.1f;
}

void camera::move_up() noexcept{
    position += up * 0.1f;
}

void camera::move_down() noexcept{
    position -= up * 0.1f;
}

glm::mat4 camera::get_view() const{
    return glm::lookAt(position, position + direction, up);
}

void camera::set_event(const move_camera& event_object) noexcept {
    if(event_object.direction != -1){ //NOTE: if direction is not set we rotate based on mouse position
        switch(event_object.direction){
            case MOVE_FORWARD:
                move_forward();
                return;
            case MOVE_BACKWARD:
                move_backward();
                return;
            case STRAFE_LEFT:
                move_left();
                return;
            case STRAFE_RIGHT:
                move_right();
                return;
            case MOVE_UP:
                move_up();
                return;
            case MOVE_DOWN:
                move_down();
                return;
            default: //NOTE: should never happen
                return;
        }
    } else{
        mouse_update(event_object.position);
        return;
    }
}

const glm::vec3& camera::get_position() const{
    return position;
}
