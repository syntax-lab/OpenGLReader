#include"text.h"

text::text(glm::vec3 origin, const std::vector<std::string>& lines): basic_object{origin}, lines_buffer_object{lines}{

}

const std::deque<std::string_view>& text::get_lines_buffer() const noexcept{
    return lines_buffer_object.get_buffer();
}

void text::update() noexcept{
    transform();
}

//TODO(SyntaX): add update handling like:
//              update lines_buffer_object when camera reach certein threshold
void text::set_event(const base_event& event) noexcept{
    if(event.type == SCROLL_TEXT_EVENT){
        const scroll_renderer& casted_event = dynamic_cast<const scroll_renderer&>(event);
        switch(casted_event.direction){
            case SCROLL_UP:
                lines_buffer_object.previous_page();
                return;
            case SCROLL_DOWN:
                lines_buffer_object.next_page();
                return;
            default: //NOTE: should never happen
                return;
        }
    }
}
