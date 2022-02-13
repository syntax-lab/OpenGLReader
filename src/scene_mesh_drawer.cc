#include<iostream>

#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>

#include"common.h"
#include"scene_mesh_drawer.h"

scene_mesh_drawer::mesh::mesh(std::vector<scene_mesh_drawer::vertex> mesh_vertices, std::vector<unsigned int> mesh_indices) noexcept:
    vertices{mesh_vertices}, indices{mesh_indices}{

    glGenVertexArrays(1, &array_buffer);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);

    glBindVertexArray(array_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(scene_mesh_drawer::vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(scene_mesh_drawer::vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(scene_mesh_drawer::vertex), COORDS_OFFSET(3));
    glBindVertexArray(0);
}

scene_mesh_drawer::mesh::~mesh() noexcept{
    unbind();

    glDeleteBuffers(1, &array_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
}

void scene_mesh_drawer::mesh::bind() const noexcept{
    glBindVertexArray(array_buffer);
}

void scene_mesh_drawer::mesh::unbind() const noexcept{
    glBindVertexArray(0);
}

GLsizei scene_mesh_drawer::mesh::get_size() const noexcept{
    return static_cast<GLsizei>(indices.size());
}

scene_mesh_drawer::scene_mesh_drawer(std::string_view filename){
    if(!load_meshs(filename)){
        std::cerr << "ERROR> failed to load 3D object!" << std::endl;
        return;
    }
}

scene_mesh_drawer::mesh scene_mesh_drawer::process_mesh(aiMesh* mesh_raw_ptr, [[maybe_unused]] const aiScene* scene_raw_ptr) const{
    std::vector<scene_mesh_drawer::vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh_raw_ptr->mNumVertices; ++i){
        scene_mesh_drawer::vertex vertex;
        glm::vec3 positions{mesh_raw_ptr->mVertices[i].x, mesh_raw_ptr->mVertices[i].y, mesh_raw_ptr->mVertices[i].z};
        vertex.position = positions;
        //TODO(SyntaX): add more normals, texture coords and so on...
        if(mesh_raw_ptr->HasNormals()){
            glm::vec3 normals{mesh_raw_ptr->mNormals[i].x, mesh_raw_ptr->mNormals[i].y, mesh_raw_ptr->mNormals[i].z};
            vertex.normals = normals;
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh_raw_ptr->mNumFaces; ++i){
        aiFace face = mesh_raw_ptr->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j){
            indices.push_back(face.mIndices[j]);
        }
    }

    return scene_mesh_drawer::mesh{vertices, indices};
}

void scene_mesh_drawer::process_node(aiNode* node_raw_ptr, const aiScene* scene_raw_ptr){
    for(unsigned int i = 0; i < node_raw_ptr->mNumMeshes; ++i){
        aiMesh* mesh = scene_raw_ptr->mMeshes[node_raw_ptr->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene_raw_ptr));
    }

    for(unsigned int i = 0; i < node_raw_ptr->mNumChildren; ++i){
        process_node(node_raw_ptr->mChildren[i], scene_raw_ptr);
    }
}

int scene_mesh_drawer::load_meshs(std::string_view filename){
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename.data(), aiProcess_Triangulate);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        //TODO(SyntaX): should return value;
        std::cerr << "ERROR> failed to import scene!" << std::endl;
        return 0;
    }

    process_node(scene->mRootNode, scene);

    return 1; //NOTE: true
}

const std::vector<scene_mesh_drawer::mesh>& scene_mesh_drawer::get_meshes() const noexcept{
    return meshes;
}

int scene_mesh_drawer::draw() const noexcept{
    for(auto&& mesh : get_meshes()){
        mesh.bind();
        glDrawElements(GL_TRIANGLES, mesh.get_size(), GL_UNSIGNED_INT, nullptr);
        mesh.unbind();
    }
    return 0; //NOTE: NO_ERROR
}
