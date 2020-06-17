#include <iostream>
#include <boost/program_options.hpp>

#include "finder.h"

std::ostream& operator<<(std::ostream& out, const std::vector<std::string> &v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>{out, " "});
  return out;
}

namespace po = boost::program_options;

int main (int argc, char ** argv) {
    
    try {
        
        Finder finder;
        
        po::options_description desc{"Bayan options"};
        desc.add_options()
                ("help,h", "This screen")
                ("dirs", po::value<std::vector<std::string>>(), "directories to scanning")
                ("except", po::value<std::vector<std::string>>(), "directories to excluded")
                ("level", po::value<size_t>()->default_value(1), "level: 1 for recursive scanning, 0 otherwise")
                ("min_size", po::value<size_t>()->default_value(1), "min file size in bytes")
                ("masks", po::value<std::vector<std::string>>(), "file masks")
                ("block_size", po::value<std::size_t>()->default_value(1), "read block size in bytes")
                ("hash", po::value<std::string>()->default_value("md5"), "hash func, must be crc32 or md5");

        po::positional_options_description p;
        p.add("dirs", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        
        if (vm.count("dirs")) {
            auto dirs = vm["dirs"].as<std::vector<std::string>>();
            finder.set_directories(dirs);
        } else {
//            finder.set_directories({"~/projects/otus_cpp/test/data"});
            std::cout << desc << '\n';
            return 0;
        }
        
        if (vm.count("except")) {
            finder.set_except_directories(vm["except"].as<std::vector<std::string>>());
        }
        
        if (vm.count("level")) {
            finder.set_level(vm["level"].as<size_t>());
        }
        
        if (vm.count("min_size")) {
            finder.set_min_size(vm["min_size"].as<size_t>());
        }
        
        if (vm.count("masks")) {
            finder.set_file_masks(vm["masks"].as<std::vector<std::string>>());
        }
        
        if (vm.count("block_size")) {
            finder.set_block_size(vm["block_size"].as<size_t>());
        }
        
        if (vm.count("hash")) {
            finder.set_hash_type(hash_type_from_string(vm["hash"].as<std::string>()));
        }
        
        finder.run();
        
        std::cout << std::endl << "=========== RESULT: ===========" << std::endl << std::endl;
        
        finder.print_duplicates();
        
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}

