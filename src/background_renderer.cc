#include"background_renderer.h"
#include"cube.h"
#include"cuboid.h"
#include"ground.h"
#include"reader.h"

//TODO(SyntaX): unify this
int background_renderer::draw(const basic_object& object) const noexcept{
    const cube* cube_object = dynamic_cast<const cube*>(&object);
    if(cube_object != nullptr){
        if(auto error_code = cube_drawer_object.draw()) return error_code;
        return 1; //NOTE: true
    }

    const cuboid* cuboid_object = dynamic_cast<const cuboid*>(&object);
    if(cuboid_object != nullptr){
        //TODO(SyntaX): implement this
        return 1; //NOTE: true
    }
    
    const reader* reader_object = dynamic_cast<const reader*>(&object);
    if(reader_object != nullptr){
        if(auto error_code = reader_drawer_object.draw()) return error_code;
        return 1; //NOTE: true
    }

    const ground* ground_object = dynamic_cast<const ground*>(&object);
    if(ground_object != nullptr){
        if(auto error_code = ground_drawer_object.draw()) return error_code;
    }

    return -1; //NOTE: passed object was not supported by this renderer
}
