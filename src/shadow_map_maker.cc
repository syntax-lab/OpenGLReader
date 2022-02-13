#include<GL/glew.h>

#include"shadow_map_maker.h"

constexpr unsigned int SHADOW_WIDTH = 4096;
constexpr unsigned int SHADW_HEIGHT = 4096;

shadow_map_maker::shadow_map_maker() noexcept{
    glGenFramebuffers(1, &depth_map_frame_buffer_id);
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[]{1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_frame_buffer_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);
}

shadow_map_maker::~shadow_map_maker() noexcept{
    detach_depth_map();

    glDeleteBuffers(1, &depth_map_frame_buffer_id);
    glDeleteBuffers(1, &depth_map);
}

void shadow_map_maker::prepare_depth_map(
    std::shared_ptr<background_renderer> background_renderer_ptr,
    const std::vector<std::shared_ptr<basic_object>>& background_objects,
    const glm::mat4& light_space_matrix) const noexcept{
    //glCullFace(GL_FRONT);
    background_renderer_ptr->set_active_shader(0);
    auto shadow_shader = background_renderer_ptr->get_active_shader();
    shadow_shader->use();
    shadow_shader->uniform("shadow_map", 0);

    background_renderer_ptr->set_active_shader(1);
    auto light_shader = background_renderer_ptr->get_active_shader();
    light_shader->use();
    light_shader->uniform("light_space_matrix", light_space_matrix);

    GLint current_viewport[4];
    glGetIntegerv(GL_VIEWPORT, current_viewport);
    auto&& [x, y, width, height] = current_viewport;

    glViewport(0, 0, SHADOW_WIDTH, SHADW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_frame_buffer_id);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto active_shader = background_renderer_ptr->get_active_shader();
    for(auto&& background_object : background_objects){
        background_object->transform();
        active_shader->uniform("model", background_object->get_model());
        background_renderer_ptr->draw(*background_object);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glCullFace(GL_BACK);
}

void shadow_map_maker::apply_depth_map() const noexcept{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_map);
}

void shadow_map_maker::detach_depth_map() const noexcept{
    glBindTexture(GL_TEXTURE_2D, 0);
}
