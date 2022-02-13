#pragma once

#include<glm/glm.hpp>

#include"basic_object.h"

class ground: public basic_object{
public:
    ground(glm::vec3 pos, glm::vec3 size = {1, 1, 1}) noexcept: basic_object{pos, size}{}
    void update() noexcept override;
};
