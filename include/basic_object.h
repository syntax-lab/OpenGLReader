#pragma once

#ifndef BASIC_OBJECT_H

    #define BASIC_OBJECT_H

    #include<glm/glm.hpp>
    #include<glm/gtx/transform.hpp>

    #include"event.h"
    #include"expandable_enum.h"

    namespace expandable_enum{
        class basic_object_status_error: public enum_status_error{
        public:
            constexpr basic_object_status_error(int new_error_status) noexcept: enum_status_error{new_error_status}{}
        };

        constexpr basic_object_status_error FAILED_TO_CREATE_PROGRAM = 64;
    }

    //TODO(SyntaX): temporary solution
    class basic_object{
    protected:
        inline static int global_id =  0; //NOTE: not used
        int id                      = -1;

        int status                  =  0; //NOTE: 0 == NO_ERROR

        glm::vec3 position{0.f};
        glm::vec3 scale{1, 1, 1};
        glm::vec3 rotation{0.f};
        float angle = 0.0f;

        glm::mat4 model{1.f};

    public:
        basic_object(
            const glm::vec3& origin,
            const glm::vec3& size = {1, 1, 1},
            const glm::vec3& rotate = {1, 1, 1},
            float rotation_angle = 0.0f) noexcept:
                position{origin}, 
                scale{size}, 
                rotation{rotate},
                angle{rotation_angle}{}

        virtual ~basic_object(){}

        //virtual void draw()                            const noexcept = 0;
        virtual void update()                                noexcept = 0;

        //virtual const program& get_shader()            const noexcept = 0;

        //template<typename... Args>
        //        void set_event(const event<Args...>&)        noexcept = 0; //NOTE: template functions cannot be virtual
        virtual void set_event(const base_event&) noexcept{

        }

        inline void transform() noexcept {
            model = glm::translate(position) * glm::scale(scale) * glm::rotate(glm::radians(angle), rotation);
        }

        inline const glm::mat4& get_model() const noexcept {
            return model;
        }

        constexpr int get_id() const noexcept { return id; }

        constexpr glm::vec3 get_position() const noexcept { return position; }

        constexpr void set_status(const expandable_enum::enum_status_error& new_status){
            status = static_cast<int>(new_status);
        }

        constexpr int get_status() const noexcept { return status; }
    };

#endif /* BASIC_OBJECT_H */
