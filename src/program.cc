#include<format>
#include<iostream>

#include"file_reader.h"
#include"program.h"

void program::error_handler() {
    switch(status){
        case NO_ERROR: return;
        case NO_SHADERS_TO_COMPILE:
            std::cerr << "ERROR> no shaders to compile!" << std::endl;
            break;
        case EMPTY_SHADER_SOURCE_FILE: //NOTE: not used
            std::cerr << std::format("ERROR> file `{}` is empty!", get_current_shader_path()) << std::endl;
            break;
        case CANNOT_CREATE_PROGRAM:
            std::cerr << "ERROR> cannot create program!" << std::endl;
            break;
        case CANNOT_OPEN_SHADER_SOURCE_FILE: //NOTE: not used
            std::cerr << std::format("ERROR> file `{}` cannot be open!", get_current_shader_path()) << std::endl;
            break;
        case CANNOT_LOAD_SHADER:
            std::cerr << std::format("ERROR> cannot load {} shader source code from `{}` file!", translate_shader_type(get_current_shader_type()), get_current_shader_path()) << std::endl;
            break;
        case CANNOT_CREATE_SHADER:
            std::cerr << std::format("ERROR> cannot create {} shader from `{}` file!", translate_shader_type(get_current_shader_type()), get_current_shader_path()) << std::endl;
            break;
        case FAILED_TO_COMPILE_SHADER:
            std::cerr << std::format("ERROR> compilation of {} shader from `{}` file failed!", translate_shader_type(get_current_shader_type()), get_current_shader_path()) << std::endl;
            break;
        case NO_SHADER_COMPILATION_INFO_LOG:
            std::cerr << std::format("ERROR> compilation of {} shader from `{}` file failed without info log!", translate_shader_type(get_current_shader_type()), get_current_shader_path()) << std::endl;
            break;
        case FAILED_TO_LINK_PROGRAM:
            std::cerr << "ERROR> failed to link program!" << std::endl;
            break;
        case NO_LINKAGE_INFO_LOG:
            std::cerr << "ERROR> program linkaged failed but info log was not provided!" << std::endl;
            break;
        case FAILED_TO_VALIDATE_PROGRAM:
            std::cerr << "ERROR> failed to validate program!" << std::endl;
            break;
        case NO_VALIDATION_INFO_LOG:
            std::cerr << "ERROR> program validation failed but info log was not provided!" << std::endl;
            break;
        case UNDEFINED_STATUS:
            std::cerr << "ERROR> during program bulding program encounter undefined error status!" << std::endl;
            break;
        default: //NOTE: should never ever happend
            break;
    }

    clear_shader_context();
    advance_build_step(BUILD_FAILED);
}

bool program::validate_input(const std::vector<std::tuple<std::string_view, GLuint>>& shaders_paths) noexcept {
    if(shaders_paths.empty()){
        set_error_status(NO_SHADERS_TO_COMPILE);
        return false;
    }
    return true;
}

GLuint program::bind_program_id() noexcept {
    GLuint internal_program_id = glCreateProgram();
    if(internal_program_id == 0){
        set_error_status(CANNOT_CREATE_PROGRAM);
        return 0;
    }
    return internal_program_id;
}

std::string program::load_shader_source(const std::string_view& shader_name) {
    file_reader file_reader_object{shader_name};

    std::string shader_source = file_reader_object.read();

    if(!file_reader_object.end_reached()){
        set_error_status(CANNOT_OPEN_FILE);
        return "";
    }

    return shader_source;
}

//NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
GLint program::try_compile_shader(const std::string_view& source_code, GLuint shader_id) const noexcept {
    GLint result = GL_FALSE;
    int log_length = 0;

    const GLchar* source_code_ptr = source_code.data();

    glShaderSource(shader_id, 1, &source_code_ptr, nullptr);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 0){
        std::string info_log(log_length, 0);
        glGetShaderInfoLog(shader_id, log_length, nullptr, info_log.data());
        std::cerr << std::format("ERROR> COMPILATION ERROR:\n{{\n{}}}", info_log) << std::endl;
        return COMPILATION_FAILED;
    }

    glAttachShader(program_id, shader_id);

    return result;
}

//NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
GLint program::try_link_program() const noexcept {
    GLint result = GL_FALSE;
    int log_length = 0;

    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 0){
        std::string info_log(log_length, 0);
        glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());
        std::cerr << std::format("ERROR> LINKAGE ERROR:\n{{\n{}}}", info_log) << std::endl;
        return LINKAGE_FAILED;
    }

    return result;
}

//NOTE: consider change return value to COMPILATION_FAILED if failed or SUCCESS if not
GLint program::try_validate_program() const noexcept {
    GLint result = GL_FALSE;
    int log_length = 0;

    glValidateProgram(program_id);

    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 0){
        std::string info_log(log_length, 0);
        glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());
        std::cerr << std::format("ERROR> VALIDATION ERROR:\n{{\n{}}}", info_log) << std::endl;
        return VALIDATION_FAILED;
    }

    return result;
}

void program::advance_build_step(build_progress new_step) noexcept {
    build_step = new_step;
}

std::string_view program::translate_shader_type(GLuint shader_type) const noexcept{
    switch(shader_type){
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        default:
            return "unknown";
    }
}

//NOTE: translate result status to specified error status
program::error_status program::translate_result_status(GLint result_status) const noexcept {
    //NOTE: ERROR_STATUS_MAP[RESULT_STATUS][BUILD_STEP];
    switch(result_status){
        case GL_TRUE:
            return NO_ERROR;
        //NOTE: if we modify try_* functions this switch is redundant
        case GL_FALSE:
            switch(build_step){
                case SHADER_COMPILATION:
                    return NO_SHADER_COMPILATION_INFO_LOG;
                case PROGRAM_LINKAGE:
                    return NO_LINKAGE_INFO_LOG;
                case PROGRAM_VALIDATION:
                    return NO_VALIDATION_INFO_LOG;
                default:
                    return UNDEFINED_STATUS;
            }
        case COMPILATION_FAILED:
            switch(build_step){
                case SHADER_COMPILATION:
                    return FAILED_TO_COMPILE_SHADER;
                default:
                    return UNDEFINED_STATUS;
            }
        case LINKAGE_FAILED:
            switch(build_step){
                case PROGRAM_LINKAGE:
                    return FAILED_TO_LINK_PROGRAM;
                default:
                    return UNDEFINED_STATUS;
            }
        case VALIDATION_FAILED:
            switch(build_step){
                case PROGRAM_VALIDATION:
                    return FAILED_TO_VALIDATE_PROGRAM;
                default:
                    return UNDEFINED_STATUS;
            }
        default:
            return UNDEFINED_STATUS;
    }
}

std::string program::translate_prompt_string(build_string_index index) const {
    switch(build_step){
        case SHADER_COMPILATION:
            switch(index){
                case COMPILATION_BEGIN_STRING:
                    return std::format("INFO> {} shader compilation of `{}` file has been started...", translate_shader_type(get_current_shader_type()), get_current_shader_path());
                case COMPILATION_END_STRING:
                    return std::format("INFO> {} shader compilation of `{}` file has been finished with status: ", translate_shader_type(get_current_shader_type()), get_current_shader_path());
                default:
                    return ""; //NOTE: should never happend
            }
        case PROGRAM_LINKAGE:
            switch(index){
                case LINKAGE_BEGIN_STRING:
                    return "INFO> program linkage has been started...";
                case LINKAGE_END_STRING:
                    return "INFO> program linkage has been finished with status: ";
                default:
                    return ""; //NOTE: should never happend
            }
        case PROGRAM_VALIDATION:
            switch(index){
                case VALIDATION_BEGIN_STRING:
                    return "INFO> program validation has been started...";
                case VALIDATION_END_STRING:
                    return "INFO> program validation has been finished with status: ";
                default:
                    return "";
            }
        default:
            return ""; //NOTE: should never happend
    }
}

bool program::check_source_file([[maybe_unused]] const std::string_view& source) noexcept {
    if(status == CANNOT_OPEN_FILE){
        set_error_status(CANNOT_LOAD_SHADER);
        return false;
    }
    return true;
}

bool program::next_build_step(){
    GLint result = GL_FALSE;
    switch(build_step){
        case SHADER_COMPILATION:
        {
            std::cout << translate_prompt_string(COMPILATION_BEGIN_STRING) << std::endl;
            //NOTE: we don't need to pass arguments to this function we can read them inside but we want to leave this function C-style like function
            auto&& source = get_current_shader_source_code();
            auto&& shader_id = get_current_shader_id();
            result = try_compile_shader(source, shader_id);
            std::cout << translate_prompt_string(COMPILATION_END_STRING) << (result == GL_TRUE?GL_FALSE:GL_TRUE) << std::endl; //NOTE: if succeeded return 0 status otherwise 1
            break;
        }
        case PROGRAM_LINKAGE:
        {
            std::cout << translate_prompt_string(LINKAGE_BEGIN_STRING) << std::endl;
            result = try_link_program();
            std::cout << translate_prompt_string(LINKAGE_END_STRING) << (result == GL_TRUE?GL_FALSE:GL_TRUE) << std::endl; //NOTE: if succeeded return 0 status otherwise 1
            break;
        }
        case PROGRAM_VALIDATION:
        {
            std::cout << translate_prompt_string(VALIDATION_BEGIN_STRING) << std::endl;
            result = try_validate_program();
            std::cout << translate_prompt_string(VALIDATION_END_STRING) << (result == GL_TRUE?GL_FALSE:GL_TRUE) << std::endl; //NOTE: if succeeded return 0 status otherwise 1
            break;
        }
        default:
        {
            set_error_status(UNDEFINED_STATUS);
            return false;
        }
    }

    if(result != GL_TRUE){
        set_error_status(translate_result_status(result));
        return false;
    }

    return true;
}

GLuint program::create_shader(const std::string_view& shader_source, GLuint shader_type){
    advance_build_step(SHADER_COMPILATION);

    GLuint shader_id = 0;

    switch(shader_type){
        case GL_VERTEX_SHADER:
            shader_id = vertex_shader();
            break;
        case GL_FRAGMENT_SHADER:
            shader_id = fragment_shader();
            break;
        default:
            std::cout << std::format("INFO> shader type attached to file `{}` is not supported!", get_current_shader_path()) << std::endl;
            return 0;
    }

    if(shader_id == 0){
        set_error_status(CANNOT_CREATE_SHADER);
        return 0;
    }

    update_shader_context(shader_source, shader_id);

    if(!next_build_step())
        return 0;

    return shader_id;
}

GLuint program::vertex_shader() const noexcept {
    return glCreateShader(GL_VERTEX_SHADER);
}

GLuint program::fragment_shader() const noexcept {
    return glCreateShader(GL_FRAGMENT_SHADER);
}

bool program::link_program() {
    advance_build_step(PROGRAM_LINKAGE);
    return next_build_step();
}

bool program::validate_program(){
    advance_build_step(PROGRAM_VALIDATION);
    return next_build_step();
}

void program::finalize_build() noexcept {
    advance_build_step(BUILD_COMPLETED);
}

GLuint program::create_program(const std::vector<std::tuple<std::string_view, GLuint>>& shaders_paths){
    //NOTE: don't need to return bool value we can just simply check using is_error_set function
    if(!validate_input(shaders_paths)){
        error_handler();
        return 0;
    }

    program_id = bind_program_id();

    if(is_error_set()){
        error_handler();
        return 0;
    }

    std::vector<GLuint> shaders_id;

    for(auto&& [shader_path, shader_type] : shaders_paths){
        create_shader_context(shader_path, shader_type);

        std::string shader_source = load_shader_source(shader_path);

        if(!check_source_file(shader_source)){
            error_handler();
            return 0;
        }

        GLuint shader_id = create_shader(shader_source, shader_type);

        if(shader_id == 0){
            if(is_error_set()){
                error_handler();
                return 0;
            }
            continue; //NOTE: not supported shader should be removed when all shaders will be supported
        }

        shaders_id.push_back(shader_id);
        //clear_shader_context();
    }

    //NOTE: don't need to return bool value we can just simply use is_error_set 
    if(!link_program()){
        error_handler();
        return 0;
    }

    //NOTE: don't need to return bool value we can just simply use is_error_set 
    if(!validate_program()){
        error_handler();
        return 0;
    }

    for(GLuint shader_id: shaders_id){
        glDetachShader(program_id, shader_id);
        glDeleteShader(shader_id);
    }

    finalize_build();

    return program_id;
}

program::program(const std::vector<std::tuple<std::string_view, GLuint>>& shaders_paths){
    program_id = create_program(shaders_paths); //NOTE: this assignment is redundant
    if(is_valid())
        std::cout << std::format("INFO> program id: {}", program_id) << std::endl;
}

program::~program() noexcept{
    clear_shader_context();
    glDeleteProgram(program_id);
}

void program::use() const noexcept{
    glUseProgram(program_id);
}
