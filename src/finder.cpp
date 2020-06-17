#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

#include "finder.h"
#include "filter.h"

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

void Finder::prepare() {
    files_map.clear();
    
    file_filters.clear();
    file_filters.push_back(std::make_unique<FilterMasks>(file_masks));
    file_filters.push_back(std::make_unique<FilterSize>(min_size));
    file_filters.push_back(std::make_unique<FilterMac>());
}

void Finder::run() {
    prepare();
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
                process_file(begin->path());
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
    if (!is_filtered(path)) {
        return;
    }
    
    std::cout << path << '\n';

    size_t size = boost::filesystem::file_size(path);
    FileData file(path.string(), size, block_size);

    std::list<std::string> l = { path.string() };
    auto p = files_map.insert(std::make_pair(file, l));
    if (!p.second) {
        auto it = p.first;
        it->second.push_back(path.string());
    }
}

bool Finder::is_filtered(const boost::filesystem::path& path) const {
    
    auto it_broken = std::find_if(file_filters.cbegin(), file_filters.cend(), [&](const std::unique_ptr<Filter>& filter){
        return !filter->is_satisfied(path);
    });
    
    return it_broken == file_filters.cend();
}

void Finder::print_duplicates(std::ostream& output) {
    for (const auto& _p : files_map) {
        //output << "size: " << _p.first.get_blocks_size() << ", filename: " << _p.first.get_filename() << std::endl;
        const auto& duplicates = _p.second;
        if (duplicates.size() > 1) {
            for (const auto& filename : duplicates) {
                output << filename << std::endl;
            }
        }
        output << std::endl;
    }
}
