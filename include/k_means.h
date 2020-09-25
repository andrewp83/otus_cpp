#pragma once

#include <vector>

#include <dlib/clustering.h>

#include "point.h"


class KMeans {
public:
    KMeans(unsigned long num_clusters, double kernel_gamma, double tolerance, unsigned long max_dictionary_size = 1000000);
    
    void fit(const std::vector<Point>& data_points);
    
    unsigned long predict(const Point& point);
    
private:
    typedef dlib::matrix<double, 2, 1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    unsigned long num_clusters {0};
    
    dlib::kcentroid<kernel_type> kc;
    dlib::kkmeans<kernel_type> test;
};
