#include "lines_buffer.h"

lines_buffer::lines_buffer(const std::vector<std::string>& lines): lines_ref{lines}, actual_buffer_size{lines.size()}{
    for(size_t line_number = 0; line_number < MAX_QUEUE_SIZE; ++line_number){
        if(line_number < actual_buffer_size){
            lines_buffer_queue.push_back(lines[line_number]);
        } else{
            lines_buffer_queue.push_back("");
        }
    }
}

void lines_buffer::move_up(){
    if(current_lines_index > 0){
        lines_buffer_queue.pop_back();
        lines_buffer_queue.push_front(lines_ref[current_lines_index - 1]);
        current_lines_index--;
    }
}

void lines_buffer::move_down(){
    if(current_lines_index + MAX_QUEUE_SIZE < actual_buffer_size){
        lines_buffer_queue.pop_front();
        lines_buffer_queue.push_back(lines_ref[current_lines_index + MAX_QUEUE_SIZE]);
        current_lines_index++;
    }
}

void lines_buffer::next_page() noexcept{
    size_t new_index = current_lines_index + MAX_QUEUE_SIZE;

    if(new_index >= actual_buffer_size) return;

    for(size_t line_number = 0; line_number < MAX_QUEUE_SIZE; ++line_number){
        size_t offset = new_index + line_number;
        if(offset < actual_buffer_size){
            lines_buffer_queue[line_number] = lines_ref[offset];
        } else{
            lines_buffer_queue[line_number] = "";
        }
    }

    current_lines_index = new_index;
}

void lines_buffer::previous_page() noexcept{
    if(current_lines_index == 0) return;

    size_t new_index = current_lines_index - MAX_QUEUE_SIZE;

    for(size_t line_number = 0; line_number < MAX_QUEUE_SIZE; ++line_number){
        size_t offset = new_index + line_number;
        lines_buffer_queue[line_number] = lines_ref[offset];
    }

    current_lines_index = new_index;
}
