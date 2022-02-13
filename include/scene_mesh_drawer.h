#pragma once

#include<string_view>
#include<vector>

#include<assimp/scene.h>

#include<GL/glew.h>
#include<glm/glm.hpp>

class scene_mesh_drawer{
    struct vertex{
        glm::vec3 position = {0, 0, 0};
        glm::vec3 normals = {0, 0, 0};
        //TODO(SyntaX): add more attributes like texture coords, normals etc.
    };

    struct mesh{
        GLuint array_buffer;
        GLuint vertex_buffer;
        GLuint index_buffer;

        std::vector<scene_mesh_drawer::vertex> vertices;
        std::vector<unsigned int> indices;

        mesh(std::vector<scene_mesh_drawer::vertex>, std::vector<unsigned int>) noexcept;

        ~mesh() noexcept;

        void bind() const noexcept;

        void unbind() const noexcept;
        
        GLsizei get_size() const noexcept;
        
    };

    std::vector<mesh> meshes;

    scene_mesh_drawer::mesh process_mesh(aiMesh* mesh_raw_ptr, [[maybe_unused]] const aiScene* scene_raw_ptr) const;

    void process_node(aiNode*, const aiScene*);

    int load_meshs(std::string_view);

public:
    scene_mesh_drawer(std::string_view);
    
    const std::vector<mesh>& get_meshes() const noexcept;

    int draw() const noexcept;
};
