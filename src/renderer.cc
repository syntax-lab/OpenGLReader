#include"basic_scene.h"
#include"renderer.h"

void renderer::render(std::shared_ptr<basic_scene> current_scene){
    current_scene->build();
}
