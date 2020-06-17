#pragma once

#include <stdlib.h>

#include "hash_value.h"

class FileBlock {
public:
    FileBlock(char* data, size_t size, HashFunc hash_func);
    
    bool operator<(const FileBlock& other) const;
    
    //std::string dump() { return std::string(data, size); }

private:
	//char* data {nullptr};
    size_t size {0};
    
    std::shared_ptr<HashValue> hash_value;
};
