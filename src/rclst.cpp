#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "flat.h"
#include "flat_clusterizator.h"
#include "utils.h"


//const std::string& FILE_DATASET = "dataset.csv";

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        std::cout << "Usage: rclst number_of_clusters model_filename" << std::endl;
        return 0;
    }
    
    unsigned long n_clusters;
    try {
        n_clusters = std::stoi(argv[1]);
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    
    std::string model_fname = argv[2];
    
    // read_data
    std::vector<Flat> data_points;
    
    //std::fstream fs(FILE_DATASET);

    for(std::string line; std::getline(std::cin/*fs*/, line);) {
        Flat flat;
        unsigned long cl;
        flat_from_str(line, flat, cl);
        data_points.push_back(flat);
    }
    
    FlatClusterizator clusterizator(n_clusters);
    clusterizator.fit(data_points);
    clusterizator.serialize(model_fname);
    
    return 0;
}
