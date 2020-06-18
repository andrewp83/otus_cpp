#pragma once

#include <fstream>
#include <list>
#include <string>

#include "hash_value.h"

class FileData {
public:
    FileData() {}
	FileData(const std::string& filename, size_t file_size, size_t block_size, HashFunc hash_func);
    
    FileData(const FileData&) = delete;
    
    FileData(FileData&& other);
    
    ~FileData();

    class Iterator {
    public:
        using value_type = HashValuePtr;
        typedef std::forward_iterator_tag iterator_category;
        
        Iterator(const FileData* data, bool is_end = false);

        HashValuePtr operator*() const;

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        Iterator& operator++();

    private:
        const FileData* data {nullptr};
        std::list<HashValuePtr>::iterator inner_iterator;
        bool is_end {false};
    };

	bool operator<(const FileData& other) const;

    size_t get_file_size() const { return file_size; }
    size_t get_read_size() const;

	Iterator begin() const;
	Iterator end() const;
    
private:
    void read_next_block() const;
    void close_file() const;

private:
	std::string filename;
    size_t file_size {0};
    size_t block_size {0};
    HashFunc hash_func;
    
    char* block_buffer {nullptr};
    
    //std::fstream fin;
    mutable FILE* fin {nullptr};
    mutable std::list<HashValuePtr> data_blocks;
};

