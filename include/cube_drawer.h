#pragma once

#include"GL/glew.h"

class cube_drawer{
    GLuint cube_array_buffer_id;
    GLuint cube_vertices_buffer_id;
    GLuint cube_index_buffer_id;

    void bind() const noexcept{
        glBindVertexArray(cube_array_buffer_id);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_bufer_id);
    }

    void unbind() const noexcept{
        glBindVertexArray(0);
    }

    GLsizei get_size() const noexcept{
        return 36;
    }

public:
    cube_drawer() noexcept{
        glGenVertexArrays(1, &cube_array_buffer_id);
        glGenBuffers(1, &cube_vertices_buffer_id);
        glGenBuffers(1, &cube_index_buffer_id);

        float cube_vertices_pos[]{
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f
        };

        unsigned int cube_indices[]{
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            7, 6, 5,
            5, 4, 7,
            4, 0, 3,
            3, 7, 4,
            4, 5, 1,
            1, 0, 4,
            3, 2, 6,
            6, 7, 3
        };

        glBindVertexArray(cube_array_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vertices_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_pos), cube_vertices_pos, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindVertexArray(0);
    }

    int draw() const noexcept{
        bind();
        glDrawElements(GL_TRIANGLES, get_size(), GL_UNSIGNED_INT, nullptr);
        unbind();
        return 0; //NOTE: NO_ERROR
    }
};
