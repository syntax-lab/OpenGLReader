#include<iostream>
#include<format>

#include"content.h"
#include"cube.h"
#include"error_codes.h"
#include"event.h"
#include"ground.h"
#include"text_and_background_scene.h"
#include"reader.h"

int content::init(){
    std::shared_ptr<audio_playback> audio_playback_ptr = std::make_shared<audio_playback>(scenes_music_themes);

    if(audio_playback_ptr == nullptr){
        std::cerr << "ERROR> cannot create audio playback!" << std::endl;
        return FAILED_TO_CREATE_AUDIO_PLAYBACK;
    }

    if(auto error_code = audio_playback_ptr->is_valid()){
        while(error_code){
            int last_active_bit = error_code & -error_code;
            switch(last_active_bit){
                case FAILED_TO_OPEN_AUDIO_DEVICE:
                    std::cerr << "ERROR> cannot open audio device!" << std::endl;
                    break;
                case FAILED_TO_CREATE_AUDIO_CONTEXT:
                    std::cerr << "ERROR> cannot create audio context!" << std::endl;
                    break;
                case FAILED_TO_GET_AUDIO_DEVICE_SPECIFIER:
                    std::cerr << "ERROR> cannot get audio device specifier!" << std::endl;
                    break;
                case FAILED_TO_SET_AUDIO_SOURCE_CONFIG:
                    std::cerr << "ERROR> cannot set audio source config!" << std::endl;
                    break;
                case FAILED_TO_SET_AUDIO_LISTENER_CONFIG:
                    std::cerr << "ERROR> cannot set audio listener config!" << std::endl;
                    break;
                case FAILED_TO_CREATE_AUDIO_DEVICE:
                    std::cerr << "ERROR> cannot create audio device!" << std::endl;
                    break;
                case FAILED_TO_CREATE_AUDIO_SOURCE:
                    std::cerr << "ERROR> cannot create audio source!" << std::endl;
                    break;
                case FAILED_TO_CREATE_AUDIO_LISTENER:
                    std::cerr << "ERROR> cannot create audio listener!" << std::endl;
                    break;
                case FAILED_TO_ADD_BACKGROUND_SOUND:
                    std::cerr << "ERROR> cannot set background sound!" << std::endl;
                    break;
            }
            error_code ^= last_active_bit;
        }

        std::cerr << "ERROR> cannot initialize audio playback!" << std::endl;
        return FAILED_TO_INITIALIZE_AUDIO_PLAYBACK;
    }

    renderer_ptr = std::make_shared<renderer>();

    if(renderer_ptr == nullptr){
        std::cerr << "ERROR> cannot create renderer!" << std::endl;
        return FAILED_TO_CREATE_RENDERER;
    }

    std::shared_ptr<program> text_renderer_program_ptr = std::make_shared<program>(
        std::vector<std::tuple<std::string_view, unsigned int>>{
        {
            {"resources/text_renderer_vertex_shader.glsl", GL_VERTEX_SHADER},
            {"resources/text_renderer_fragment_shader.glsl", GL_FRAGMENT_SHADER}
        }
    });

    std::shared_ptr<program> background_renderer_program_ptr = std::make_shared<program>(
        std::vector<std::tuple<std::string_view, unsigned int>>{
            {
                {"resources/background_renderer_vertex_shader.glsl", GL_VERTEX_SHADER},
                {"resources/background_renderer_fragment_shader.glsl", GL_FRAGMENT_SHADER}
            }
    });

    std::shared_ptr<program> light_space_program_ptr = std::make_shared<program>(
        std::vector<std::tuple<std::string_view, unsigned int>>{
            {
                {"resources/light_space_vertex_shader.glsl", GL_VERTEX_SHADER}
            }
    });
    
    if(text_renderer_program_ptr == nullptr || !text_renderer_program_ptr->is_valid() ||
       background_renderer_program_ptr == nullptr || !background_renderer_program_ptr->is_valid() || 
       light_space_program_ptr == nullptr || !light_space_program_ptr->is_valid()){
        std::cerr << "ERROR> cannot create renderer programs!" << std::endl;
        return FAILED_TO_CREATE_PROGRAMS;
    }

    std::shared_ptr<text_renderer> text_renderer_ptr = std::make_shared<text_renderer>();
    
    if(text_renderer_ptr == nullptr || !text_renderer_ptr->is_valid()){
        std::cerr << "ERROR> cannot create text renderer!" << std::endl;
        return FAILED_TO_CREATE_TEXT_RENDERER;
    }

    text_renderer_ptr->add_program(text_renderer_program_ptr);

    std::shared_ptr<background_renderer> background_renderer_ptr = std::make_shared<background_renderer>();

    if(background_renderer_ptr == nullptr || !background_renderer_ptr->is_valid()){
        std::cerr << "ERROR> cannot create background renderer!" << std::endl;
        return FAILED_TO_CREATE_BACKGROUND_RENDERER;
    }

    background_renderer_ptr->add_program(background_renderer_program_ptr);
    background_renderer_ptr->add_program(light_space_program_ptr);

    for(unsigned int scene_id = 0; scene_id < SCENE_NUMBER; ++scene_id){
        set_current_scene(scene_id);
        auto& current_scene = get_current_scene();

        std::cout << std::format("INFO> scene number: {}, initialization...", scene_id + 1) << std::endl;

        current_scene = std::make_shared<text_and_background_scene>(scenes_texts[scene_id], audio_playback_ptr, text_renderer_ptr, background_renderer_ptr);

        if(current_scene == nullptr){
            std::cerr << "ERROR> cannot create scene pointer!" << std::endl;
            return FAILED_TO_CREATE_SCENE;
        }

        if(auto error_code = current_scene->is_valid()){
            while(error_code){
                int last_active_bit = error_code & -error_code;
                switch(last_active_bit){
                    case FAILED_TO_READ_INPUT_FILE:
                        std::cerr << "ERROR> cannot read input file!" << std::endl;
                        break;
                    case FAILED_TO_CREATE_PROGRAMS:
                        std::cerr << "ERROR> cannot create shader programs!" << std::endl;
                        break;
                    case FAILED_TO_LOAD_TEXTURE:
                        std::cerr << "ERROR> cannot load texture image!" << std::endl;
                        break;
                    case FAILED_TO_INITIALIZE_FONT_LIBRARY:
                        std::cerr << "ERROR> cannot initialize free type library!" << std::endl;
                        break;
                    case FAILED_TO_CREATE_FONT_FACE:
                        std::cerr << "ERROR> cannot create font face!" << std::endl;
                        break;
                }
                error_code ^= last_active_bit;
            }
            return FAILED_TO_INITIALIZE_SCENE;
        }

        //TODO(SyntaX): move it to background class and pass to constructor or something?
        switch(scene_id){
            case 0:{
                current_scene->add_object(std::make_shared<cube>(glm::vec3{-30.0f, -150.0f, 0.0f}, 10));
                current_scene->add_object(std::make_shared<cube>(glm::vec3{-60.0f, -150.0f, 0.0f}, 10, glm::vec3{0, 1, 0}, 45.0f));
                current_scene->add_object(std::make_shared<cube>(glm::vec3{-45.0f, -121.5f, 15.0f}, 10));
                current_scene->add_object(std::make_shared<cube>(glm::vec3{170.0f, -150.0f, 20.0f}, 10, glm::vec3{0, 1, 0}, -45.0f));
                current_scene->add_object(std::make_shared<reader>(glm::vec3{50, -80, -10}, glm::vec3{150, 100, 10}));
                current_scene->add_object(std::make_shared<ground>(glm::vec3{0, -175, 0}, glm::vec3{500, 500, 500}));
                break;
            }
            case 1: [[fallthrough]];
            case 2: [[fallthrough]];
            case 3: [[fallthrough]];
            case 4:
                current_scene->add_object(std::make_shared<reader>(glm::vec3{50, -80, -10}, glm::vec3{150, 100, 10}));
                break;
        }
    }

    set_current_scene(0);

    return NO_ERROR;
}

void content::set_current_scene(unsigned int new_scene_id) noexcept{
    current_scene_id = new_scene_id;
}

std::shared_ptr<basic_scene>& content::get_current_scene() noexcept{
    return scenes_renderer_ptr[current_scene_id];
}

void content::render_scene() noexcept{
    auto& current_scene = get_current_scene();
    renderer_ptr->render(current_scene);
}

void content::previous_scene() noexcept{
    if(current_scene_id > 0){
        current_scene_id -= 1;
        get_current_scene()->update(scene_change{SCENE_PREVIOUS});
    }
}

void content::next_scene() noexcept{
    if(current_scene_id < SCENE_NUMBER - 1){
        current_scene_id += 1;
        get_current_scene()->update(scene_change{SCENE_NEXT});
    }
}

void content::start_audio_playback(){
    get_current_scene()->audio_start();
}

void content::volume_up() noexcept{
    get_current_scene()->volume_up();
}

void content::volume_down() noexcept{
    get_current_scene()->volume_down();
}

void content::volume_mute() noexcept{
    get_current_scene()->volume_mute();
}

void content::start_stop_playing() noexcept{
    get_current_scene()->audio_start_stop();
}
