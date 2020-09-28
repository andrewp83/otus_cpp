
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "flat.h"
#include "flat_clusterizator.h"
#include "utils.h"


int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cout << "Usage: rclss model_filename" << std::endl;
        return 0;
    }
        
    std::string model_fname = argv[1];
    
    FlatClusterizator clusterizator(model_fname);
    
    for(std::string line; std::getline(std::cin, line);) {
        Flat flat;
        unsigned long cl;
        flat_from_str(line, flat, cl);
        std::vector<Flat> flats = clusterizator.find_nearest_flats(flat);
        
        for (const auto& fl : flats) {
            std::string flat_str;
            flat_str += std::to_string(fl.longitude); flat_str += ";";
            flat_str += std::to_string(fl.latitude); flat_str += ";";
            flat_str += std::to_string(fl.rooms); flat_str += ";";
            flat_str += std::to_string(fl.price); flat_str += ";";
            flat_str += std::to_string(fl.footage); flat_str += ";";
            flat_str += std::to_string(fl.kitchen); flat_str += ";";
            flat_str += std::to_string(fl.floor); flat_str += ";";
            flat_str += std::to_string(fl.floors);
            std::cout << flat_str << std::endl;
        }
    }
    
    getchar();
    
    return 0;
}
