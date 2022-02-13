#pragma once

#include<glm/glm.hpp>

#include"basic_object.h"
#include"event.h"

class cuboid: public basic_object{
public:
    cuboid(glm::vec3, glm::vec3, glm::vec3, float);
    
    void update() noexcept override;
    void set_event(const base_event&) noexcept override;
};