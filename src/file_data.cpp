#include "file_data.h"

#include <algorithm>
#include <iostream>


FileData::FileData(const std::string& filename, size_t file_size, size_t block_size, HashFunc hash_func)
: filename(filename)
, file_size(file_size)
, block_size(block_size)
, hash_func(hash_func) {
    
    block_buffer = new char[block_size];
}

FileData::FileData(FileData&& other)
: filename(std::move(other.filename))
, file_size(std::move(other.file_size))
, block_size(std::move(other.block_size))
, hash_func(std::move(other.hash_func))
, fin(std::move(other.fin))
, data_blocks(std::move(other.data_blocks)) {

    block_buffer = other.block_buffer;
    other.block_buffer = nullptr;
}

FileData::~FileData() {
    close_file();
    delete block_buffer;
}

bool FileData::operator<(const FileData& other) const {
    auto comp_less = [](const HashValuePtr& l, const HashValuePtr& r) {
        return l->less(r.get());
    };
    bool compare = std::lexicographical_compare(begin(), end(), other.begin(), other.end(), comp_less);
    close_file();
    return compare;
}

size_t FileData::get_read_size() const {
    return data_blocks.size() * block_size;
}

void FileData::read_next_block() const {
    if (!fin) {
        fin = fopen(filename.c_str(), "r");
        fseek(fin, get_read_size(), SEEK_SET);
    }
    
    fread(block_buffer, block_size, 1, fin);
    
    data_blocks.push_back(HashValue::create(hash_func, block_buffer, block_size));
}

void FileData::close_file() const {
    if (fin) {
        fclose(fin);
        fin = nullptr;
    }
}

FileData::Iterator FileData::begin() const {
	return FileData::Iterator(this);
}

FileData::Iterator FileData::end() const {
    return FileData::Iterator(this, true);
}


// File::Iterator

FileData::Iterator::Iterator(const FileData* data, bool is_end):
data(data),
is_end(is_end) {
    
    if (!is_end) {
        if (data->data_blocks.empty()) {
            data->read_next_block();
        }
        inner_iterator = data->data_blocks.begin();
    } else {
        inner_iterator = data->data_blocks.end();
    }
}

HashValuePtr FileData::Iterator::operator*() const {
	return *inner_iterator;
}

bool FileData::Iterator::operator==(const Iterator& other) const {
    if (is_end && other.is_end) {
        return true;
    }
	return inner_iterator == other.inner_iterator;
}

bool FileData::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

FileData::Iterator& FileData::Iterator::operator++() {
    
    inner_iterator++;
    
    if (inner_iterator == data->data_blocks.end()) {
        if (data->get_read_size() < data->get_file_size()) {
            data->read_next_block();
            inner_iterator = std::prev(data->data_blocks.end());
        } else {
            is_end = true;
        }
	}
    
	return *this;
}
