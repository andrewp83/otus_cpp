#pragma once

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

#include "common_types.h"
#include "filter.h"
#include "file_data.h"

namespace boost {
namespace filesystem {
class path;
};
};

class Finder {
public:
	void run();
    
    void print_duplicates(std::ostream& output = std::cout);

	void set_directories(const std::vector<std::string>& directories);
	void set_except_directories(const std::vector<std::string>& directories);
	void set_level(size_t level);
	void set_min_size(size_t min_size);
	void set_file_masks(const std::vector<std::string>& masks);
	void set_block_size(size_t size);
	void set_hash_type(HashFunc hash);

private:
    void prepare();
    
    void process_dir(const std::string& path);
    void process_file(const boost::filesystem::path& path);
    
    bool is_filtered(const boost::filesystem::path& path) const;

private:
	std::vector<std::string> directories;
	std::unordered_set<std::string> except_directories;
	size_t level {0};
	size_t min_size {1};
	std::vector<std::string> file_masks;
	size_t block_size {1};
	HashFunc hash {HashFunc::MD5};
    
    std::list<std::unique_ptr<Filter>> file_filters;
    
    std::map<FileData, std::list<std::string>> files_map;
};
