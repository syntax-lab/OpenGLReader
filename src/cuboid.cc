#include"cuboid.h"

cuboid::cuboid(glm::vec3 origin, glm::vec3 size, glm::vec3 rotate, float rotation_angle): basic_object{origin, size, rotate, rotation_angle}{

}

void cuboid::update() noexcept{
    transform();
}

void cuboid::set_event([[maybe_unused]] const base_event& event) noexcept{

}
