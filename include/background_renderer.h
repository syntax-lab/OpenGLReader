#pragma once

#include"basic_object.h"
#include"basic_renderer.h"
#include"cube_drawer.h"
#include"scene_mesh_drawer.h"

class background_renderer: public basic_renderer{
    //cube_drawer cube_buffer_binder;
    scene_mesh_drawer cube_drawer_object{"resources/simple_cube.obj"};
    scene_mesh_drawer reader_drawer_object{"resources/reader.obj"};
    scene_mesh_drawer ground_drawer_object{"resources/ground.obj"};

public: 
    int draw(const basic_object&) const noexcept override;
};
