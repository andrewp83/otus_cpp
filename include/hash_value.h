#pragma once

#include <memory>

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include "common_types.h"

class HashValue {
public:
    static std::shared_ptr<HashValue> create(HashFunc func, char* buffer, size_t count);
    
    virtual bool less(const HashValue* other) const = 0;
    
    virtual ~HashValue() {}
};


class HashValueMd5 : public  HashValue {
public:
    HashValueMd5(char* buffer, size_t count);
    
    virtual bool less(const HashValue* other) const override;
    
private:
    boost::uuids::detail::md5::digest_type digest;
};


class HashValueCrc32 : public HashValue {
public:
    HashValueCrc32(char* buffer, size_t count);
    
    virtual bool less(const HashValue* other) const override;
    
private:
    boost::crc_32_type::value_type digest;
};
