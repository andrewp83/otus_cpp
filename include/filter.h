#pragma once

#include <vector>
#include <string>

namespace boost {
namespace filesystem {
class path;
};
};


class Filter {
public:
	virtual ~Filter() {}

	virtual bool is_satisfied(const boost::filesystem::path& path) const = 0;
};


class FilterMasks : public Filter {
public:
	FilterMasks(const std::vector<std::string>& masks) : masks(masks) {}

	virtual bool is_satisfied(const boost::filesystem::path& path) const override;

protected:
	std::vector<std::string> masks;
};


class FilterSize : public Filter {
public:
	FilterSize(size_t min_size) : min_size(min_size) {}

	virtual bool is_satisfied(const boost::filesystem::path& path) const override;

protected:
	size_t min_size {0};
};


class FilterMac : public Filter {
public:
    virtual bool is_satisfied(const boost::filesystem::path& path) const override;
};
