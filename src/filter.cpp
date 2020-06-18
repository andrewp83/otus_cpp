#include "filter.h"

#include <boost/filesystem.hpp>

FilterMasks::FilterMasks(const std::vector<std::string>& masks) {
    for (const auto& mask : masks) {
        this->masks.push_back(mask);
        regexp_masks.push_back(boost::regex(mask));
    }
}

bool FilterMasks::is_satisfied(const boost::filesystem::path& path) const {
    
    if (regexp_masks.empty()) {
        return true;
    }
    
    auto it = std::find_if(regexp_masks.cbegin(), regexp_masks.cend(), [&](const auto& regex) {
        boost::smatch what;
        return boost::regex_match(path.filename().string(), what, regex);
    });
    
	return it != regexp_masks.cend();
}


bool FilterSize::is_satisfied(const boost::filesystem::path& path) const {
    auto size = boost::filesystem::file_size(path);
	return size >= min_size;
}


bool FilterMac::is_satisfied(const boost::filesystem::path& path) const {
    return path.string().find(".DS_Store") == std::string::npos;
}
