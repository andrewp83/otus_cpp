#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "flat.h"
#include "k_means.h"

// ВСПОМНИМ ПЕРВУЮ ДОМАШКУ
std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}


const std::string& file_dataset = "data/dataset.csv";

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
    
    std::fstream fs(file_dataset);

    for(std::string line; std::getline(fs, line);) {
        std::vector<std::string> v = split(line, ';');
        Flat flat;
        flat.longitude = std::stod(v.at(0));
        flat.latitude = std::stod(v.at(1));
        flat.rooms = std::stoi(v.at(2));
        flat.price = std::stod(v.at(3));
        flat.footage = std::stod(v.at(4));
        flat.kitchen = std::stod(v.at(5));
        flat.floor = std::stod(v.at(6));
        flat.floors = std::stod(v.at(7));
        
        data_points.push_back(flat);
    }
    
    KMeans k_means(n_clusters, 0.1, 0.01);
    
    k_means.fit(data_points);
    
    k_means.serialize(model_fname);
    
//    for (const Point& pt : data_points) {
//        unsigned long cluster = k_means.predict(pt);
//        std::cout << pt.x << ";" << pt.y << ";" << cluster << std::endl;
//    }

    
    return 0;
}
