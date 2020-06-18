#include "filter.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

FilterMasks::FilterMasks(const std::vector<std::string>& masks) {
    for (const auto& mask : masks) {
        this->masks.push_back(mask);
        regexp_masks.push_back(boost::regex(boost::algorithm::to_lower_copy(mask)));
    }
}

bool FilterMasks::is_satisfied(const boost::filesystem::path& path) const {
    
    if (regexp_masks.empty()) {
        return true;
    }
    
    auto it = std::find_if(regexp_masks.cbegin(), regexp_masks.cend(), [&](const auto& regex) {
        boost::smatch what;
        std::string path_str = boost::algorithm::to_lower_copy(path.filename().string());
        return boost::regex_match(path_str, what, regex);
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
