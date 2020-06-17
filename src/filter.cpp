#include "filter.h"

#include <boost/filesystem.hpp>


bool FilterMasks::is_satisfied(const boost::filesystem::path& path) const {
    (void)path;
	return true;
}


bool FilterSize::is_satisfied(const boost::filesystem::path& path) const {
    (void)path;
	return true;
}


bool FilterMac::is_satisfied(const boost::filesystem::path& path) const {
    return path.string().find(".DS_Store") == std::string::npos;
}
