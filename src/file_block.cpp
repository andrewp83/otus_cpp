#include "file_block.h"

#include <cstring>

bool FileBlock::operator<(const FileBlock& other) const {
    if (size < other.size) {
        return true;
    }
    int cmp = memcmp(data, other.data, size);
    return cmp < 0;
}
