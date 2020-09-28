//
//  flat_clusterizator.hpp
//  realty
//
//  Created by a.polyakov on 28.09.2020.
//

#pragma once

#include "flat.h"
#include "k_means.h"


class FlatClusterizator {
public:
    FlatClusterizator(unsigned long num_clusters);
    FlatClusterizator(const std::string& model_fname);
    
    template<class FlatContainer>
    void fit(const FlatContainer& flats) {
        std::vector<DataRow> flats_data;
        for (const auto& fl : flats) {
            DataRow row(7);
            flat_to_data_row(fl, row);
            flats_data.push_back(row);
        }
        
        clusterizator.fit(flats_data);
        
        flat_clusters.clear();
        
        for (std::size_t index = 0; index < flats.size(); index++) {
            unsigned long cluster = clusterizator.predict(flats_data[index]);
            const Flat& fl = flats[index];
            flat_clusters.emplace_back(fl, cluster);
        }
        
        // ОТСОРТИРОВАТЬ ПО КЛАСТЕРАМ
        std::sort(flat_clusters.begin(),
                  flat_clusters.end(),
                  [](const std::pair<Flat, unsigned long>& l, const std::pair<Flat, unsigned long>& r) {
                      return l.second < r.second;
                  }
        );
    }
    
    unsigned long predict(const Flat& flat) const;
    
    std::vector<Flat> find_nearest_flats(const Flat& flat) const;
    
    void serialize(const std::string& modelfname);
    void deserialize(const std::string& modelfname);
    
private:
    static void flat_to_data_row(const Flat& flat, DataRow& row);
    
    static double flats_distance(const Flat& flat_1, const Flat& flat_2);
    
private:
    KMeans clusterizator;
    std::vector<std::pair<Flat, unsigned long>> flat_clusters;
};
