#include"program.h"

template<size_t I, size_t J, GLboolean Transpose> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
void program::uniform_impl(const GLchar* const name, const GLsizei count, const GLint* const value) const noexcept {
    GLint location = glGetUniformLocation(program_id, name);
    if constexpr(J != 1) static_assert("uniform matrix for integer types is not supported!");
    if constexpr(I == 1){
        glUniform1iv(location, count, value);
    } else if constexpr(I == 2){
        glUniform2iv(location, count, value);
    } else if constexpr(I == 3){
        glUniform3iv(location, count, value);
    } else if constexpr(I == 4){
        glUniform4iv(location, count, value);
    }
}

template<size_t I, size_t J, GLboolean Transpose> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
void program::uniform_impl(const GLchar* const name, const GLsizei count, const GLuint* const value) const noexcept {
    GLint location = glGetUniformLocation(program_id, name);
    if constexpr(J != 1) static_assert("uniform matrix for integer types is not supported!");
    if constexpr(I == 1){
        glUniform1uiv(location, count, value);
    } else if constexpr(I == 2){
        glUniform2uiv(location, count, value);
    } else if constexpr(I == 3){
        glUniform3uiv(location, count, value);
    } else if constexpr(I == 4){
        glUniform4uiv(location, count, value);
    }
}

template<size_t I, size_t J, GLboolean Transpose> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
void program::uniform_impl(const GLchar* const name, const GLsizei count, const GLfloat* const value) const noexcept {
    GLint location = glGetUniformLocation(program_id, name);
    if constexpr(J == 1) {
        if constexpr(I == 1){
            glUniform1fv(location, count, value);
        } else if constexpr(I == 2){
            glUniform2fv(location, count, value);
        } else if constexpr(I == 3){
            glUniform3fv(location, count, value);
        } else if constexpr(I == 4){
            glUniform4fv(location, count, value);
        }
    } else if constexpr(J == 2){
        if constexpr(I == 2){
            glUniformMatrix2fv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3x2fv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4x2fv(location, count, Transpose, value);
        }
    } else if constexpr(J == 3){
        if constexpr(I == 2){
            glUniformMatrix2x3fv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3fv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4x3fv(location, count, Transpose, value);
        }
    } else if constexpr(J == 4){
        if constexpr(I == 2){
            glUniformMatrix2x4fv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3x4fv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4fv(location, count, Transpose, value);
        }
    }
}

template<size_t I, size_t J, GLboolean Transpose> requires concept_impl::valid_size_range<I> && concept_impl::valid_size_range<J>
void program::uniform_impl(const GLchar* const name, const GLsizei count, const GLdouble* const value) const noexcept {
    GLint location = glGetUniformLocation(program_id, name);
    if constexpr(J == 1) {
        if constexpr(I == 1){
            glUniform1dv(location, count, value);
        } else if constexpr(I == 2){
            glUniform2dv(location, count, value);
        } else if constexpr(I == 3){
            glUniform3dv(location, count, value);
        } else if constexpr(I == 4){
            glUniform4dv(location, count, value);
        }
    } else if constexpr(J == 2){
        if constexpr(I == 2){
            glUniformMatrix2dv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3x2dv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4x2dv(location, count, Transpose, value);
        }
    } else if constexpr(J == 3){
        if constexpr(I == 2){
            glUniformMatrix2x3dv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3dv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4x3dv(location, count, Transpose, value);
        }
    } else if constexpr(J == 4){
        if constexpr(I == 2){
            glUniformMatrix2x4dv(location, count, Transpose, value);
        } else if constexpr(I == 3){
            glUniformMatrix3x4dv(location, count, Transpose, value);
        } else if constexpr(I == 4){
            glUniformMatrix4dv(location, count, Transpose, value);
        }
    }
}

template<concept_impl::uniform_type Type>
inline void program::uniform(const std::string_view& name, const Type value) const noexcept {
    uniform_impl<1, 1>(name.data(), 1, &value);
}

//why Intellisense doesn't see 2 following implemetnations???
template<concept_impl::uniform_type Element_type, size_t I>
inline void program::uniform(const std::string_view& name, const glm::vec<I, Element_type>& value) const noexcept {
    uniform_impl<I, 1>(name.data(), 1, &value[0]);
}

template<concept_impl::uniform_type Element_type, size_t I, size_t J, GLboolean Transpose>
inline void program::uniform(const std::string_view& name, const glm::mat<I, J, Element_type>& value) const noexcept {
    uniform_impl<I, J, Transpose>(name.data(), 1, &value[0][0]);
}

template<concept_impl::uniform_type Type, size_t Size>
inline void program::uniform(const std::string_view& name, const Type(&values)[Size]) const noexcept {
    uniform_impl<1, 1>(name.data(), Size, &values[0]);
}

template<concept_impl::uniform_type Element_type, size_t I, size_t Size>
inline void program::uniform(const std::string_view& name, const glm::vec<I, Element_type>(&values)[Size]) const noexcept {
    uniform_impl<I, 1>(name.data(), Size, &values[0][0]);
}

template<concept_impl::uniform_type Element_type, size_t I, size_t J, size_t Size, GLboolean Transpose>
inline void program::uniform(const std::string_view& name, const glm::mat<I, J, Element_type>(&values)[Size]) const noexcept {
    uniform_impl<I, J, Transpose>(name.data(), Size, &values[0][0][0]);
}
