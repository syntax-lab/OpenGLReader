#pragma once

#ifndef BASIC_SCENE_H

    #define BASIC_SCENE_H

    #include<memory>

    #include<glm/glm.hpp>
    #include<glm/gtx/transform.hpp>

    #include"audio_playback.h"
    #include"basic_object.h"
    #include"camera.h"

    class basic_scene{
    protected:
        glm::mat4         projection_matrix;

        camera            camera_object;

        std::shared_ptr<audio_playback> audio_playback_ptr;

    public:
        basic_scene(std::shared_ptr<audio_playback> audio_playback_bind_ptr):
            projection_matrix{glm::perspective(glm::radians(45.f), 4.f / 3, 0.01f, 200.f)},
            camera_object{glm::vec3{70, -75, 190}},
            audio_playback_ptr{audio_playback_bind_ptr}{
        
        }

        void volume_up() noexcept;
        void volume_down() noexcept;
        void volume_mute() noexcept;
        void audio_start();
        void audio_start_stop() noexcept;

        virtual void build() noexcept = 0;
        virtual void add_object(std::shared_ptr<basic_object>){}
        virtual void update(const base_event&) noexcept = 0;
        virtual int is_valid() const = 0;
    };

#endif /* BASIC_SCENE_H */
