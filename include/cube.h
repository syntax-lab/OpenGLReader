#pragma once

#include<glm/glm.hpp>

#include"basic_object.h"
#include"event.h"

class cube: public basic_object{
public:
    cube(glm::vec3, unsigned int, glm::vec3 = {1, 1, 1}, float = 0.0f) noexcept;

    void update() noexcept override;
    void set_event(const base_event&) noexcept override;
};
