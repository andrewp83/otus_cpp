//
//  flat_clusterizator.cpp
//  realty
//
//  Created by a.polyakov on 28.09.2020.
//

#include <fstream>

#include "flat_clusterizator.h"


FlatClusterizator::FlatClusterizator(unsigned long num_clusters)
: clusterizator(num_clusters, 0.000001, 0.01) {
    
}

FlatClusterizator::FlatClusterizator(const std::string& model_fname) {
    deserialize(model_fname);
}

unsigned long FlatClusterizator::predict(const Flat& flat) const {
    DataRow row(7);
    flat_to_data_row(flat, row);
    unsigned long cluster = clusterizator.predict(row);
    return cluster;
}

void FlatClusterizator::serialize(const std::string& modelfname) {
    clusterizator.serialize(modelfname);
    
    std::ofstream fs;
    fs.open(modelfname + ".csv");
    std::string flat_str;
    for (const auto& _p : flat_clusters) {
        if (!flat_str.empty()) {
            fs << std::endl;
            flat_str.clear();
        }
        const Flat& flat = _p.first;
        flat_str += std::to_string(flat.longitude); flat_str += ";";
        flat_str += std::to_string(flat.latitude); flat_str += ";";
        flat_str += std::to_string(flat.rooms); flat_str += ";";
        flat_str += std::to_string(flat.price); flat_str += ";";
        flat_str += std::to_string(flat.footage); flat_str += ";";
        flat_str += std::to_string(flat.kitchen); flat_str += ";";
        flat_str += std::to_string(flat.floor); flat_str += ";";
        flat_str += std::to_string(flat.floors); flat_str += ";";
        flat_str += std::to_string(_p.second); // КЛАСТЕР
        fs << flat_str;
    }
    fs.close();
}

void FlatClusterizator::deserialize(const std::string& modelfname) {
    clusterizator.deserialize(modelfname);
    
    std::ifstream fs(modelfname + ".csv");
    
    flat_clusters.clear();

    for(std::string line; std::getline(fs, line);) {
        Flat flat;
        unsigned long cluster;
        flat_from_str(line, flat, cluster);
        flat_clusters.emplace_back(flat, cluster);
    }
}

void FlatClusterizator::flat_to_data_row(const Flat &flat, DataRow &row) {
    row[0] = flat.longitude;
    row[1] = flat.latitude;
    row[2] = flat.rooms;
    row[3] = flat.price;
    row[4] = flat.footage;
    row[5] = flat.kitchen;
    row[6] = (flat.floor == 1 || flat.floor == flat.floors) ? 0 : -1; //  0 - если этаж первый или последний, в остальных случаях - 1
}

std::vector<Flat> FlatClusterizator::find_nearest_flats(const Flat& flat) const {
    std::vector<Flat> flats;
    
    unsigned long cluster = predict(flat);
    
    auto comp = [](const std::pair<Flat, unsigned long>& l, const std::pair<Flat, unsigned long>& r){
        return l.second < r.second;
    };
    auto _p = std::equal_range(flat_clusters.begin(), flat_clusters.end(), std::make_pair(Flat(), cluster), comp);
    for (auto it = _p.first; it != _p.second; it++) {
        flats.push_back(it->first);
    }
    
    auto distance_comp = [&](const Flat& l, const Flat& r) {
        return flats_distance(l, flat) < flats_distance(r, flat);
    };
    
    std::sort(flats.begin(), flats.end(), distance_comp);
    
    return flats;
}

double FlatClusterizator::flats_distance(const Flat &flat_1, const Flat &flat_2) {
    double dist = sqrt(pow(flat_1.latitude - flat_2.latitude, 2) + pow(flat_1.longitude - flat_2.longitude, 2));
    return dist;
}
