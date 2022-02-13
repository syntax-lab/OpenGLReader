#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>

#include<concepts>
#include<string_view>
#include<tuple>
#include<vector>

#define COMPILATION_FAILED -1
#define LINKAGE_FAILED     -3
#define VALIDATION_FAILED  -5

namespace concept_impl{
    template<typename Type, typename... Valid_types>
    concept is_any_of_type = (std::same_as<Type, Valid_types> || ...);

    template<size_t N>
    concept valid_size_range = (N >= 1 && N <= 4);

    template<typename Type>
    concept uniform_type = is_any_of_type<std::remove_cv_t<Type>, 
        int, unsigned int, float, double>; //TODO(SyntaX): narrow number of types to basic types only, pointers are not necessary
    //concept valid_uniform_type = is_any_of_type<std::remove_cvref_t<std::remove_pointer_t<std::decay_t<Type>>>, int, unsigned int, float, double, int*, unsigned int*, float*, double*>;
}

class program{
    enum class error_status{
        NO_ERROR,
        NO_SHADERS_TO_COMPILE,
        CANNOT_OPEN_FILE, //mid state
        EMPTY_SHADER_SOURCE_FILE,
        CANNOT_CREATE_PROGRAM,
        CANNOT_OPEN_SHADER_SOURCE_FILE,
        CANNOT_LOAD_SHADER,
        CANNOT_CREATE_SHADER,
        FAILED_TO_COMPILE_SHADER,
        NO_SHADER_COMPILATION_INFO_LOG,
        FAILED_TO_LINK_PROGRAM,
        NO_LINKAGE_INFO_LOG,
        FAILED_TO_VALIDATE_PROGRAM,
        NO_VALIDATION_INFO_LOG,

        UNDEFINED_STATUS = 0xFFFF
    };

    enum class build_progress{
        PROGRAM_INITIALIZATION,
        SHADER_COMPILATION,
        PROGRAM_LINKAGE,
        PROGRAM_VALIDATION,
        BUILD_COMPLETED,
        BUILD_FAILED
    };

    enum class build_string_index{
        COMPILATION_BEGIN_STRING,
        COMPILATION_END_STRING,
        LINKAGE_BEGIN_STRING,
        LINKAGE_END_STRING,
        VALIDATION_BEGIN_STRING,
        VALIDATION_END_STRING
    };

    using enum error_status;
    using enum build_progress;
    using enum build_string_index;

    error_status   status = NO_ERROR;
    build_progress build_step = PROGRAM_INITIALIZATION; //NOTE: 0 - vertex shader; 1 - fragment shader // 2 - linkage
    GLuint         program_id = 0;
    
    struct shader{
        std::string_view shader_path;
        std::string_view source_code;
        GLuint           shader_type = 0;
        GLuint           id          = 0;
    };
   
    shader         current_shader;

    void error_handler();

    bool validate_input(const std::vector<std::tuple<std::string_view, GLuint>>& shaders_paths) noexcept;

    GLuint bind_program_id() noexcept;

    std::string load_shader_source(const std::string_view& shader_name);

    //NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
    GLint try_compile_shader(const std::string_view& source_code, GLuint shader_id) const noexcept;

    //NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
    GLint try_link_program() const noexcept;

    //NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
    GLint try_validate_program() const noexcept;

    void advance_build_step(build_progress new_step) noexcept;

    std::string_view translate_shader_type(GLuint shader_type) const noexcept;
 
    //NOTE: translate result status to specified error status
    error_status translate_result_status(GLint result_status) const noexcept;

    std::string translate_prompt_string(build_string_index index) const;

    bool check_source_file(const std::string_view& source) noexcept;

    bool next_build_step();

    GLuint create_shader(const std::string_view& shader_source, GLuint shader_type);

    GLuint vertex_shader() const noexcept;
    GLuint fragment_shader() const noexcept;

    bool link_program();

    bool validate_program();

    void finalize_build() noexcept;

    GLuint create_program(const std::vector<std::tuple<std::string_view, GLuint>>&);

    template<size_t I, size_t J, GLboolean Transpose = GL_FALSE> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
    void uniform_impl(const GLchar* const, const GLsizei, const GLint* const) const noexcept;
    template<size_t I, size_t J, GLboolean Transpose = GL_FALSE> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
    void uniform_impl(const GLchar* const, const GLsizei, const GLuint* const) const noexcept;
    template<size_t I, size_t J, GLboolean Transpose = GL_FALSE> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
    void uniform_impl(const GLchar* const, const GLsizei, const GLfloat* const) const noexcept;
    template<size_t I, size_t J, GLboolean Transpose = GL_FALSE> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
    void uniform_impl(const GLchar* const, const GLsizei, const GLdouble* const) const noexcept;

    inline void set_error_status(error_status error_status) noexcept {
        status = error_status;
    }

    inline bool is_error_set() const noexcept {
        return status != NO_ERROR;
    }

    inline void set_current_shader_path(const std::string_view& shader_path) noexcept {
        current_shader.shader_path = shader_path;
    }

    inline std::string_view get_current_shader_path() const noexcept {
        return current_shader.shader_path;
    }

    inline void set_current_shader_type(GLuint shader_type) noexcept {
        current_shader.shader_type = shader_type;
    }

    inline GLuint get_current_shader_type() const noexcept {
        return current_shader.shader_type;
    }

    inline void set_current_shader_source_code(const std::string_view& source) noexcept {
        current_shader.source_code = source;
    }

    inline std::string_view get_current_shader_source_code() const noexcept {
        return current_shader.source_code;
    }

    inline void set_current_shader_id(GLuint new_id) noexcept {
        current_shader.id = new_id;
    }

    inline GLuint get_current_shader_id() const noexcept {
        return current_shader.id;
    }

    inline void create_shader_context(const std::string_view& shader_path, GLuint shader_type) noexcept {
        current_shader.shader_path = shader_path;
        current_shader.shader_type = shader_type;
    }

    inline void update_shader_context(const std::string_view& shader_code, GLuint shader_id) noexcept {
        current_shader.source_code = shader_code;
        current_shader.id = shader_id;
    }

    inline void clear_shader_context() noexcept {
        current_shader.shader_path = "";
        current_shader.shader_type = 0;
        current_shader.source_code = "";
        current_shader.id = 0;
    }


public:
    program(const std::vector<std::tuple<std::string_view, GLuint>>&);
    ~program() noexcept;

    void use() const noexcept;

    inline GLuint get_id() const noexcept {
        return program_id;
    }

    inline bool is_valid() const noexcept{
        return program_id != 0;
    }

    template<concept_impl::uniform_type Type>
    void uniform(const std::string_view&, const Type) const noexcept;

    template<concept_impl::uniform_type Element_type, size_t I>
    void uniform(const std::string_view&, const glm::vec<I, Element_type>&) const noexcept;

    template<concept_impl::uniform_type Element_type, size_t I, size_t J, GLboolean Transpose = GL_FALSE>
    void uniform(const std::string_view&, const glm::mat<I, J, Element_type>&) const noexcept;

    template<concept_impl::uniform_type Type, size_t Size>
    void uniform(const std::string_view&, const Type(&)[Size]) const noexcept;

    template<concept_impl::uniform_type Element_type, size_t I, size_t Size>
    void uniform(const std::string_view&, const glm::vec<I, Element_type>(&)[Size]) const noexcept;

    template<concept_impl::uniform_type Element_type, size_t I, size_t J, size_t Size, GLboolean Transpose = GL_FALSE>
    void uniform(const std::string_view&, const glm::mat<I, J, Element_type>(&)[Size]) const noexcept;
};

#include"program.inl"
//template void program::uniform<1>(const std::string_view, const GLsizei, const GLdouble) const noexcept;
//template void program::uniform<2>(const std::string_view, const GLsizei, const concept_impl::uniform_type auto);
//template void program::uniform<3>(const std::string_view, const GLsizei, const concept_impl::uniform_type auto);
//template void program::uniform<4>(const std::string_view, const GLsizei, const concept_impl::uniform_type auto);
