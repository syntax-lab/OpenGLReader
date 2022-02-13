#include"common.h"
#include"error_codes.h"
#include"text_and_background_scene.h"

text_and_background_scene::text_and_background_scene(
    std::string_view filename,
    std::shared_ptr<audio_playback> audio_playback_bind_ptr,
    std::shared_ptr<text_renderer> text_renderer_bind_ptr,
    std::shared_ptr<background_renderer> background_renderer_bind_ptr):
    basic_scene(audio_playback_bind_ptr),
    file_reader_object{filename},
    //NOTE:      position: x,    y,    scale;            read_lines: enable line wrapping
    text_object{glm::vec3{0.0f, 0.0f, 0.005f}, file_reader_object.read_lines(true)},
    shadow_map_maker_object{},
    text_renderer_ptr{text_renderer_bind_ptr},
    background_renderer_ptr{background_renderer_bind_ptr}{

}

void text_and_background_scene::add_object(std::shared_ptr<basic_object> new_object){
    background_objects.push_back(new_object);
}

void text_and_background_scene::build() noexcept{
    //TODO(SyntaX): tweak those data
    glm::vec3 light_position{10.0f, 10.0f, 10.0f};
    glm::mat4 light_projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.01f, 400.0f);
    glm::mat4 light_view = glm::lookAt(light_position, glm::vec3{0.0f, -15.0f, 0.0f}, glm::vec3{0.0, 1.0, 0.0});
    glm::mat4 light_space_matrix = light_projection * light_view;

    //TODO(SyntaX): should we recalculate depth map every frame???
    shadow_map_maker_object.prepare_depth_map(background_renderer_ptr, background_objects, light_space_matrix);
    
    auto view_projection_matrix = projection_matrix * camera_object.get_view();
    
    background_renderer_ptr->set_active_shader(0);
    auto background_shader = background_renderer_ptr->get_active_shader();
    background_shader->use();
    for(auto&& background_object: background_objects){
        background_object->update();
        background_shader->uniform("mvp", view_projection_matrix * background_object->get_model());
        background_shader->uniform("model", background_object->get_model());
        background_shader->uniform("view", camera_object.get_view());
        background_shader->uniform("shadow_map", 0);
        background_shader->uniform("view_position", camera_object.get_position());
        background_shader->uniform("light_position", light_position);
        background_shader->uniform("light_space_matrix", light_space_matrix);
        background_shader->uniform("sky_color", glm::vec3{SKY_COLOR});
        shadow_map_maker_object.apply_depth_map();
        background_renderer_ptr->draw(*background_object);
    }

    text_renderer_ptr->set_active_shader(0);
    auto text_shader = text_renderer_ptr->get_active_shader();
    text_object.update();
    text_shader->use();
    text_shader->uniform("model", text_object.get_model());
    text_shader->uniform("view", camera_object.get_view());
    text_shader->uniform("sky_color", glm::vec3{SKY_COLOR});
    text_shader->uniform("mvp", view_projection_matrix * text_object.get_model());
    text_renderer_ptr->draw(text_object);
}

void text_and_background_scene::update(const base_event& event_data) noexcept{
    if(event_data.type == MOVE_CAMERA_EVENT){
        camera_object.set_event(dynamic_cast<const move_camera&>(event_data));
    } else if(event_data.type == SCROLL_TEXT_EVENT){
        text_object.set_event(dynamic_cast<const scroll_renderer&>(event_data));
    } else if(event_data.type == SCENE_CHANGE_EVENT){
        audio_playback_ptr->set_event(dynamic_cast<const scene_change&>(event_data));
    }

    //TODO(SyntaX): implement buffer scrolling
    //auto&& camera_position = camera_object.get_position();
}

int text_and_background_scene::is_valid() const {
    int error_code = 0;
    if(!file_reader_object.end_reached()) error_code |= FAILED_TO_READ_INPUT_FILE;
    if(!text_renderer_ptr->is_valid())    error_code |= text_renderer_ptr->get_status();
    return error_code;
}
