#pragma once

#ifndef BASIC_RENDERER_H

    #define BASIC_RENDERER_H

    #include<memory>
    #include<vector>

    #include<GL/glew.h>

    #include"basic_object.h"
    #include"common.h"
    #include"program.h"

    class basic_renderer{
    protected:
        std::vector<std::shared_ptr<program>> programs;
        unsigned int current_program_idx = 0;

        int status = 0; //NOTE: NO_ERROR

    public:
        virtual ~basic_renderer(){}

        inline void set_current_program_idx(unsigned int new_program_idx) noexcept{
            if(new_program_idx >= programs.size()) return;
            current_program_idx = new_program_idx;
        }

        inline unsigned int get_current_program_idx() const noexcept{
            return current_program_idx;
        }

        inline void add_program(std::shared_ptr<program> new_program){
            programs.push_back(new_program);
        }

        inline void set_active_shader(unsigned int new_shader_idx) noexcept{
            set_current_program_idx(new_shader_idx);
        }

        inline std::shared_ptr<program> get_active_shader() const noexcept{
            return programs[get_current_program_idx()];
        }

        constexpr int get_status() const noexcept{
            return status;
        }

        constexpr void set_status(const expandable_enum::enum_status_error& new_status) noexcept{
            status = static_cast<int>(new_status);
        }

        inline int is_valid() const noexcept{
            return status == static_cast<int>(expandable_enum::NO_ERROR);
        }

        virtual int draw(const basic_object&) const noexcept = 0;
    };

#endif /* BASIC_RENDERER_H */