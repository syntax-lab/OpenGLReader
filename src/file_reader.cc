#include<cwctype>
#include<filesystem>

#include"file_reader.h"

constexpr size_t WORD_WRAPPING_THRESHOLD = 80;

file_reader::file_reader(const std::string_view& file_path): path{file_path}, file_handle{file_path}{
    if(std::filesystem::exists(file_path)){
        file_size = std::filesystem::file_size(file_path);
    }
}

//TODO(Syntax): test this
std::vector<std::string>& file_reader::read_lines(bool enable_line_wrapping){
    std::string line;

    while(file_handle.good()){
        std::getline(file_handle, line);

        //NOTE: if 0 characters were extracted we need to check if it was empty line or line with only new line character
        if(line.size() == 0){
            if(file_handle.good()){
                lines.push_back("");
            }
            continue;
        }

        size_t beginning_index = 0;

        //NOTE: line wrapping function recreates notepad.exe line wrapping
        if(enable_line_wrapping){
            if(line.size() > WORD_WRAPPING_THRESHOLD){
                size_t threshold = WORD_WRAPPING_THRESHOLD;
                
                while(threshold < line.size()){
                    size_t delimiter_index = threshold; //NOTE: at first we assume that we will every character until threshold [beginning_index, threshold)

                    //NOTE: we have to check if next character after threshold is whitespace but not to consume it
                    for(size_t index = beginning_index; index < threshold + 1; ++index){
                        if(std::iswspace(line[index])){
                            delimiter_index = index == threshold? index: index + 1; //NOTE: if delimiter is within threshold then skip whitespace otherwise don't
                        }
                    }

                    lines.push_back(line.substr(beginning_index, delimiter_index - beginning_index));
                    beginning_index = delimiter_index;
                    threshold = beginning_index + WORD_WRAPPING_THRESHOLD;
                }
            }
        }

        lines.push_back(line.substr(beginning_index));
    }

    number_of_lines = lines.size();

    return lines;
}

//TODO(SyntaX): temporarily we read only whole file
std::string file_reader::read(size_t bytes_to_read){
    std::string data;

    file_handle.unsetf(std::ios::skipws); //NOTE: maybe whitespaces are not required so might be deleted (note refer to shader source files)

    std::copy<std::istream_iterator<char>>({file_handle}, {}, std::back_inserter(data)); 

    return data;
}
