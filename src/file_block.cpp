#include "file_block.h"

FileBlock::FileBlock(char* data, size_t size, HashFunc hash_func)
: /*data(data)
, */size(size) {
    hash_value = HashValue::create(hash_func, data, size);
}

bool FileBlock::operator<(const FileBlock& other) const {
    if (size != other.size) {
        return size < other.size;
    }
    bool cmp = hash_value->less(other.hash_value.get());
    return cmp;
}

