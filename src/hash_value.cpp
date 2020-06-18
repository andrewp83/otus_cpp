#include "hash_value.h"

HashValuePtr HashValue::create(HashFunc func, char* buffer, size_t count) {
    if (func == HashFunc::CRC32) {
        return std::make_shared<HashValueCrc32>(buffer, count);
    } else if (func == HashFunc::MD5) {
        return std::make_shared<HashValueMd5>(buffer, count);
    }
    return nullptr;
}


// CRC32 HASH
    
HashValueCrc32::HashValueCrc32(char* data, size_t size) {
    boost::crc_32_type result;
    result.process_bytes(data, size);
    digest = result.checksum();
}

bool HashValueCrc32::less(const HashValue* other) const {
    return digest < ((HashValueCrc32*)other)->digest;
}


// MD5 HASH
    
HashValueMd5::HashValueMd5(char* data, size_t size) {
    boost::uuids::detail::md5 hash;

    hash.process_bytes(data, size);
    hash.get_digest(digest);
}

bool HashValueMd5::less(const HashValue* other) const {
    bool res = std::lexicographical_compare(std::begin(digest),
                                            std::end(digest),
                                            std::begin(((HashValueMd5*)other)->digest),
                                            std::end(((HashValueMd5*)other)->digest));
    return res;
}
