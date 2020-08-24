#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "mapper.h"


class MailString {
public:
    MailString(const std::string& data, std::size_t compared_chars) : data(data), compared_chars(compared_chars) {}
    
    bool operator<(const MailString& other) const;
    bool operator==(const MailString& other) const;
    
    const std::string& get_data() const { return data; }
    
protected:
    std::string data;
    std::size_t compared_chars{3};
};

using MailContainer = std::vector<MailString>;


class MailMapper : public mr::IMapper<MailString, MailContainer> {
public:
    MailMapper(const std::string& filename, std::size_t compared_chars) : filename(filename), compared_chars(compared_chars) {}
    
	virtual MailContainer call(std::size_t chunk_num, std::size_t chunks_count) override;
    
private:
    std::string filename;
    std::mutex file_m;
    std::size_t compared_chars {2};
};
