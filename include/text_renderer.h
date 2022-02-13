#pragma once
#include<unordered_map>

#include<GL/glew.h>

#include"basic_object.h"
#include"basic_renderer.h"
#include"glyph.h"

namespace expandable_enum{
    class text_renderer_status_error: public enum_status_error{
    public:
        constexpr text_renderer_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
    };

    constexpr text_renderer_status_error FAILED_TO_LOAD_TEXTURE            = 128;
    constexpr text_renderer_status_error FAILED_TO_INITIALIZE_FONT_LIBRARY = 256;
    constexpr text_renderer_status_error FAILED_TO_CREATE_FONT_FACE        = 512;
}

class text_renderer: public basic_renderer{
    std::unordered_map<GLchar, glyph> glyphs_map;

    GLuint array_buffer_id;
    GLuint vertices_buffer_id;

    inline void bind_buffers() const noexcept {
        glBindVertexArray(array_buffer_id);
    }

    inline void unbind_buffers() const noexcept {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

public:
    text_renderer();
    ~text_renderer() noexcept;

    int draw(const basic_object&) const noexcept override;
};
