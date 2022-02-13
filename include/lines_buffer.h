#pragma once

#ifndef LINES_BUFFER_H
    #define LINES_BUFFER_H
    #include<deque>
    #include<string>
    #include<string_view>
    #include<vector>

    constexpr size_t MAX_QUEUE_SIZE = 65;

    //TODO(SyntaX): change deque to circular buffer and check performance
    class lines_buffer{
        const std::vector<std::string>& lines_ref;
        std::deque<std::string_view> lines_buffer_queue;
        size_t current_lines_index = 0; //NOTE: indicate upper bound buffer index 
        size_t actual_buffer_size  = 0;

    public:
        lines_buffer(const std::vector<std::string>&);

        void move_up();
        void move_down();

        //NOTE: never use with lines_buffer::move_up && lines_buffer::move_down function 
        //      because lines_buffer::next_page && lines_buffer::previous_page 
        //      strongly relay on current_line_index align which might be broken by them.
        void next_page() noexcept;
        void previous_page() noexcept;

        inline const std::deque<std::string_view>& get_buffer() const noexcept { return lines_buffer_queue; }
    };
#endif /* LINES_BUFFER_H */
