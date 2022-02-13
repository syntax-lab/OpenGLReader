#pragma once

#include<memory>
#include"basic_scene.h"

class renderer{
public:
    void render(std::shared_ptr<basic_scene>);
};
