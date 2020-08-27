#include "mail_reducer.h"

#include <algorithm>
#include <iostream>


void MailReducer::call(const MailContainer& chunk, std::size_t chunk_num) {
    
    if (chunk.empty()) {
        save_to_file(-1, chunk_num);
    }
    
    auto it_shortest = std::min_element(chunk.begin(), chunk.end(), [](const MailString& left, const MailString& right) {
        return left.get_data().size() < right.get_data().size();
    });
    
    std::size_t min_length = it_shortest->get_data().size();
    
    std::size_t compared_length = 1;
    
    auto comparator = [&compared_length](const MailString& left, const MailString& right) {
        int cmp = left.get_data().compare(0, compared_length, right.get_data(), 0, compared_length);
        return cmp == 0;
    };
    
    while (compared_length <= min_length) {
        auto it = std::adjacent_find(chunk.begin(), chunk.end(), comparator);
        if (it == chunk.end()) {
            break;
        }
        compared_length++;
    }
    
    int min_prefix_length = (compared_length <= min_length) ? compared_length : -1;
    save_to_file(min_prefix_length, chunk_num);
}

void MailReducer::save_to_file(int chars_num, std::size_t suffix) {
    
    std::string file_name = "reduce_" + std::to_string(suffix) + ".txt";
    
    FILE* f = fopen(file_name.c_str(), "w");
    std::string str = std::to_string(chars_num);
    fwrite(str.c_str(), str.size(), 1, f);
    fclose(f);
}
