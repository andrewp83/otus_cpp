#pragma once

#include <fstream>
#include <list>
#include <string>

#include "file_block.h"

class FileData {
public:
    FileData() {}
	FileData(const std::string& filename, size_t file_size, size_t block_size, HashFunc hash_func);
    ~FileData();

	class Iterator;

	bool operator<(const FileData& other) const;

	size_t get_file_size() const { return file_size; }
    size_t get_blocks_size() const { return data_blocks.size(); }
    const std::string& get_filename() const { return filename; }

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
    
    //std::fstream fin;
    mutable FILE* fin {nullptr};
	mutable std::list<FileBlock> data_blocks;
};

class FileData::Iterator {
public:
    using value_type = FileBlock;
    
	Iterator(const FileData* data, bool is_end = false);

	FileBlock& operator*() const;

	bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

	Iterator& operator++();

private:
	const FileData* data {nullptr};
	std::list<FileBlock>::iterator inner_iterator;
    bool is_end {false};
};
