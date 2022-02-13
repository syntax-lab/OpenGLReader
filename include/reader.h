#pragma once

#include<glm/glm.hpp>

#include"basic_object.h"
#include"event.h"

class reader: public basic_object{
public:
    reader(glm::vec3 pos, glm::vec3 size = {1, 1, 1}) noexcept: basic_object{pos, size}{}
    void update() noexcept override;
    //void set_event(const base_event&) noexcept override;
};
