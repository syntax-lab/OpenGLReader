#include"cube.h"

cube::cube(glm::vec3 origin, unsigned int size, glm::vec3 rotate, float rotation_angle) noexcept: basic_object{origin, glm::vec3{size, size, size}, rotate, rotation_angle}{

}

void cube::update() noexcept{
    transform();
}

void cube::set_event([[maybe_unused]] const base_event& event) noexcept{

}
