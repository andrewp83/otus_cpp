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
    
    // Найти начало email-a
    if (chunk_num > 0) {
        do {
            fread(&ch, 1, 1, fp);
        } while (ch != '\n');
    }
    
    std::size_t current_read = 0;
    std::string current_buffer;
    bool is_last_chunk = (chunk_num == (chunks_count - 1));
    while ( (current_read++ < chunk_size) || ((ch != '\n') && !is_last_chunk) ) {
        fread(&ch, 1, 1, fp);
        if (ch == '\n') {
            cont.push_back(current_buffer);
            //std::cout << current_buffer << std::endl;
            current_buffer.clear();
        } else {
            current_buffer.push_back(ch);
        }
    }
        
    fclose(fp);
    
    std::sort(cont.begin(), cont.end());
    
	return cont;
}
