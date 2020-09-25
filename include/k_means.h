#pragma once

#include <vector>
#include <unordered_map>

#include <dlib/clustering.h>

#include "flat.h"

// НАДО КОНЕЧНО СДЕЛАТЬ НЕЗАВИСИМЫМ ОТ FLAT, ТОЛЬКО КОГДА((
class KMeans {
public:
    KMeans(unsigned long num_clusters, double kernel_gamma, double tolerance, unsigned long max_dictionary_size = 1000000);
    
    void fit(const std::vector<Flat>& data_points);
    
    unsigned long predict(const Flat& fl);
    
    void serialize(const std::string& file_name);
    void deserialize(const std::string& file_name);
    
private:
    typedef dlib::matrix<double, 7, 1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    unsigned long num_clusters {0};
    
    dlib::kcentroid<kernel_type> kc;
    dlib::kkmeans<kernel_type> test;
    
    std::unordered_map<long, sample_type::type> rows_min_values;
    std::unordered_map<long, sample_type::type> rows_max_values;
};
