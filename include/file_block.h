#pragma once

#include <stdlib.h>

class FileBlock {
public:
	FileBlock(char* data, size_t size) : data(data), size(size) {}

	char* get_data() const { return data; }
    
    bool operator<(const FileBlock& other) const;

private:
	char* data {nullptr};
    size_t size {0};
};

class FileBlockCrc32 {
public:
    FileBlockCrc32(char* data, size_t size) : data(data), size(size) {}
    
    bool operator<(const FileBlockCrc32& other) const;

private:
    char* data {nullptr};
    size_t size {0};
};
