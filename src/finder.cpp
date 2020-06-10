#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

#include "finder.h"

void Finder::set_directories(const std::vector<std::string>& directories) {
    this->directories = directories;
}

void Finder::set_except_directories(const std::vector<std::string>& directories) {
    except_directories.clear();
    std::copy(directories.begin(), directories.end(), std::inserter(except_directories, except_directories.end()));
}

void Finder::set_level(size_t level) {
    this->level = level;
}

void Finder::set_min_size(size_t min_size) {
    this->min_size = min_size;
}

void Finder::set_file_masks(const std::vector<std::string>& masks) {
    this->file_masks = masks;
    for (auto& mask : file_masks) {
        boost::algorithm::replace_all(mask, ".", "\\.");
        boost::algorithm::replace_all(mask, "?", ".");
        boost::algorithm::replace_all(mask, "*", ".*");
        mask = std::string("^") + mask + "$";
    }
}

void Finder::set_block_size(size_t size) {
    this->block_size = size;
}

void Finder::set_hash_type(HashFunc hash) {
    this->hash = hash;
}

void Finder::run() {
    std::for_each(directories.cbegin(), directories.cend(), [this](const std::string& path) {
        process_dir(path);
    });
}

void Finder::process_dir(const std::string& path) {
    boost::filesystem::directory_iterator begin(path);
    boost::filesystem::directory_iterator end;

    for (; begin != end; ++ begin) {
        
        boost::filesystem::file_status fs = begin->status();

        switch (fs.type()) {
            case boost::filesystem::regular_file:
                process_file(begin->path().string());
                break;
            case boost::filesystem::directory_file:
                if (level > 0) {
                    process_dir(begin->path().string());
                }
                break;
            default:
                break;
        }
    }
}

void Finder::process_file(const boost::filesystem::path& path) {
    if (is_filtered(path)) {
        std::cout << path << '\n';
    }
}

bool Finder::is_filtered(const boost::filesystem::path& path) const {
    
    auto it_broken = std::find_if(filters.cbegin(), filters.cend(), [&](const unique_ptr<Filter> filter){
        return !filter->is_satisfy(path);
    });
    
    bool found = false;
    for (auto& mask : file_masks) {
        boost::regex expr(mask);
        const std::string& filename = path.filename().string();
        if (boost::regex_match(filename, expr)) {
            found = true;
            break;
        }
    }
    
    return found;
}
