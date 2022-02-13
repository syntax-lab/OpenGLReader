#include<iostream>
#include<format>

#include<ft2build.h>
#include<freetype/freetype.h>
#include<glm/glm.hpp>

//#include"image_loader.h"
#include"text.h"
#include"text_renderer.h"


//TODO(SyntaX): leading size should be mutable
constexpr double  LEADING_SIZE = 2.5; 
constexpr FT_UInt FONT_SIZE = 512;

text_renderer::text_renderer(){
    glGenVertexArrays(1, &array_buffer_id);
    glGenBuffers(1, &vertices_buffer_id);
    //glGenTextures(1, &texture_id);

    //int width = 0;
    //int height = 0;
    //int channels = 0;

    //stbi_set_flip_vertically_on_load(true);

    //stbi_uc* image_data = stbi_load("resized_texture_1.png", &width, &height, &channels, 0);
    //if(!image_data){
    //    status = static_cast<int>(text_renderer_status_error::FAILED_TO_LOAD_TEXTURE);
    //    return;
    //}

    FT_Library free_type_library;
    if(FT_Init_FreeType(&free_type_library)){
        set_status(expandable_enum::FAILED_TO_INITIALIZE_FONT_LIBRARY);
        return;
    }

    //TODO(SyntaX): Gothic3.ttf font doesn't work with newest truetype font library :(
    FT_Face font_face;
    if(FT_New_Face(free_type_library, "resources/consola.ttf", 0, &font_face)){
        FT_Done_FreeType(free_type_library);
        set_status(expandable_enum::FAILED_TO_CREATE_FONT_FACE);
        return;
    };

    FT_Set_Pixel_Sizes(font_face, 0, FONT_SIZE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char glyph_id = 32; glyph_id < 128; glyph_id++){
        if(FT_Load_Char(font_face, glyph_id, FT_LOAD_RENDER)){
            std::cerr << std::format("INFO> cannot load glyph: {}.", static_cast<int>(glyph_id)) << std::endl;
            continue;
        }

        GLuint glyph_texture_id;
        glGenTextures(1, &glyph_texture_id);
        glBindTexture(GL_TEXTURE_2D, glyph_texture_id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            font_face->glyph->bitmap.width,
            font_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            font_face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glyphs_map[glyph_id] = {
            glyph_texture_id,
            glm::ivec2(font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows),
            glm::ivec2(font_face->glyph->bitmap_left, font_face->glyph->bitmap_top),
            static_cast<unsigned int>(font_face->glyph->advance.x)
        };
    }

    FT_Done_Face(font_face);
    FT_Done_FreeType(free_type_library);

    //glBindTexture(GL_TEXTURE_2D, texture_id);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //stbi_image_free(image_data);

    glBindVertexArray(array_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), COORDS_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), COORDS_OFFSET(2));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), COORDS_OFFSET(4));

    //NOTE: reset to default value
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

text_renderer::~text_renderer() noexcept{
    unbind_buffers();

    glDeleteBuffers(1, &array_buffer_id);
    glDeleteBuffers(1, &vertices_buffer_id);
}

//NOTE: we need: member_pos: vec3, member_color: vec3, text_member: string_view
int text_renderer::draw(const basic_object& object) const noexcept{
    const text* text_object_ptr = dynamic_cast<const text*>(&object);
    
    if(!text_object_ptr) return 0; //NOTE: true

    get_active_shader()->uniform("glyph_texture", 0);
    get_active_shader()->uniform("glyph_foreground", 1);
    get_active_shader()->uniform("glyph_color", glm::vec3{1, 0.5, 0.25});

    bind_buffers();

    auto origin = object.get_position();

    float glyph_x     = origin.x;
    float glyph_y     = origin.y;
    float glyph_scale = origin.z;

    for(auto&& line: text_object_ptr->get_lines_buffer()){
        float step = 1.f / line.size();
        float character_number = 0;
        for(auto&& single_glyph: line){
            if(!glyphs_map.contains(single_glyph)){
                std::cerr << std::format("INFO> glyph: `{}`({}) is not predefined!", single_glyph, static_cast<int>(single_glyph)) << std::endl;
                continue;
            }

            const glyph& character = glyphs_map.at(single_glyph);
            float xpos = glyph_x + character.bearing.x * glyph_scale;
            float ypos = glyph_y - (character.size.y - character.bearing.y) * glyph_scale;
            float w = character.size.x * glyph_scale;
            float h = character.size.y * glyph_scale;

            float vertices_coords[6][6]{
                { xpos,     ypos + h,   0.0f, 0.0f, character_number, 1},
                { xpos,     ypos,       0.0f, 1.0f, character_number, 0},
                { xpos + w, ypos,       1.0f, 1.0f, character_number + step, 0},

                { xpos,     ypos + h,   0.0f, 0.0f, character_number, 1},
                { xpos + w, ypos,       1.0f, 1.0f, character_number + step, 0},
                { xpos + w, ypos + h,   1.0f, 0.0f, character_number + step, 1}
            };

            character_number += step;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, character.texture_id);
            //glActiveTexture(GL_TEXTURE1);
            //glBindTexture(GL_TEXTURE_2D, texture_id);

            glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_id);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_coords), vertices_coords);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            glyph_x += (character.advance >> 6) * glyph_scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glyph_y -= LEADING_SIZE; //NOTE: leading
        glyph_x = 0;
    }

    unbind_buffers();

    return 1; //NOTE: true
}
