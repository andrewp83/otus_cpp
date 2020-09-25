#include <iostream>
#include <vector>

#include "k_means.h"
#include "point.h"


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


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: kkmeans number_of_clusters" << std::endl;
        return 0;
    }
    
    unsigned long n_clusters;
    try {
        n_clusters = std::stoi(argv[1]);
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    
    // read_data
    std::vector<Point> data_points;

    for(std::string line; std::getline(std::cin, line);) {
        std::vector<std::string> v = split(line, ';');
        data_points.emplace_back(std::stod(v.at(0)), std::stod(v.at(1)));
    }
    
    //KMeans k_means(n_clusters, 0.1, 0.01, 8);
    KMeans k_means(n_clusters, 0.1, 0.01);
    
    k_means.fit(data_points);
    
    for (const Point& pt : data_points) {
        unsigned long cluster = k_means.predict(pt);
        std::cout << pt.x << ";" << pt.y << ";" << cluster << std::endl;
    }
    
    return 0;
}


