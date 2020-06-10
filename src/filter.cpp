#include "filter.h"


bool FilterMasks::is_satisfied(const boost::filesystem::path& path) const {
	return true;
}

bool FilterSize::is_satisfied(const boost::filesystem::path& path) const {
	return true;
}