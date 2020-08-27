#include "mail_mapper.h"

#include <iostream>
#include <filesystem>


MailContainer MailMapper::call(std::size_t chunk_num, std::size_t chunks_count) {
    std::lock_guard<std::mutex> lock(file_m);
    
	MailContainer cont;

    std::filesystem::path p{filename};
    std::size_t filesize = std::filesystem::file_size(p);
    FILE* fp = fopen(filename.c_str(), "r");
    std::size_t chunk_size = filesize / chunks_count;
    long offset = chunk_size * chunk_num;
    fseek(fp, offset, SEEK_SET);
    
    char ch;

    std::size_t current_read = 0;
    
    // Найти начало email-a
    if (chunk_num > 0) {
        fseek(fp, -1, SEEK_CUR);
        fread(&ch, 1, 1, fp);
        while (ch != '\n') {
            fread(&ch, 1, 1, fp);
            current_read++;
        }
    }
    
    std::string current_buffer;
    bool is_last_chunk = (chunk_num == (chunks_count - 1));
    while ( (current_read++ < chunk_size) || ((ch != '\n') && !is_last_chunk) || (is_last_chunk && !feof(fp)) ) {
        fread(&ch, 1, 1, fp);
        if (ch == '\n' || feof(fp)) {
            cont.push_back(MailString(current_buffer, compared_chars));
//            std::cout << chunk_num << ", " << current_buffer << std::endl;
            current_buffer.clear();
        } else {
            current_buffer.push_back(ch);
        }
    }
        
    fclose(fp);
    
    std::sort(cont.begin(), cont.end());
    
	return cont;
}

// MailString

bool MailString::operator<(const MailString &other) const {
    auto it_lhs_end = data.begin();
    std::advance(it_lhs_end, std::min(compared_chars, data.size()));
    
    auto it_rhs_end = other.data.begin();
    std::advance(it_rhs_end, std::min(compared_chars, other.data.size()));
    
    return std::lexicographical_compare(data.begin(), it_lhs_end, other.data.begin(), it_rhs_end);
}

bool MailString::operator==(const MailString &other) const {
    return !(*this < other) && !(other < *this);
}
