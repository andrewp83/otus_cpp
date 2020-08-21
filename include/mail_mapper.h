#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "mapper.h"

using MailContainer = std::vector<std::string>;

class MailMapper : public mr::IMapper<MailContainer> {
public:
    MailMapper(const std::string& filename) : filename(filename) {}
    
	virtual MailContainer call(std::size_t chunk_num, std::size_t chunks_count) override;
    
private:
    std::string filename;
    std::mutex file_m;
};
