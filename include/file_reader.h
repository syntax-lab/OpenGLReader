#pragma once

#ifndef FILE_READER_H
    #define FILE_READER_H
    
    #include<fstream>
    #include<limits>
    #include<string>
    #include<string_view>
    #include<vector>

    class file_reader{
        std::string_view path;
        std::fstream file_handle;
        size_t file_size       = 0;
        size_t number_of_lines = 0;
        std::vector<std::string> lines;

    public:
        file_reader(const std::string_view&);

        //TODO(Syntax): implement file_reader::read function allowing user to read arbitrary amount of data
        std::string read(size_t = std::numeric_limits<size_t>::max());
        //NOTE: set to true if you want enable line wrapping
        std::vector<std::string>& read_lines(bool);
        inline bool end_reached() const { return file_handle.eof(); } //NOTE: We read all characters at once thus we only check if we hit end of file
        inline const std::vector<std::string>& get_lines() const noexcept { return lines; }
    };
#endif /* FILE_READER_H */
