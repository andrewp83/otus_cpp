#include "k_means.h"

KMeans::KMeans(unsigned long num_clusters, double kernel_gamma, double tolerance, unsigned long max_dictionary_size)
: num_clusters(num_clusters)
, kc(kernel_type(kernel_gamma), tolerance, max_dictionary_size)
, test(kc) {
    
}

void KMeans::fit(const std::vector<Flat>& data_points) {
    
    std::vector<sample_type> samples;
    sample_type m;
    
    // НОРМАЛЬИЗОВАТЬ НЕ ЗАБЫТЬ
    
    rows_minmax_values.clear();

    for (const auto& fl : data_points) {
        m(0) = fl.longitude;
        m(1) = fl.latitude;
        m(2) = fl.rooms;
        m(3) = fl.price;
        m(4) = fl.footage;
        m(5) = fl.kitchen;
        m(6) = (fl.floor == 1 || fl.floor == fl.floors) ? 0 : -1; //  0 - если этаж первый или последний, в остальных случаях - 1
        samples.push_back(m);
    }
    
    std::vector<sample_type> initial_centers;
    
    // tell the kkmeans object we made that we want to run k-means with k set to 3.
    // (i.e. we want 3 clusters)
    test.set_number_of_centers(num_clusters);

    // You need to pick some initial centers for the k-means algorithm.  So here
    // we will use the dlib::pick_initial_centers() function which tries to find
    // n points that are far apart (basically).
    pick_initial_centers(num_clusters, initial_centers, samples, test.get_kernel());

    // now run the k-means algorithm on our set of samples.
    test.train(samples, initial_centers);
}

unsigned long KMeans::predict(const Flat& fl) {
    sample_type m;
    m(0) = fl.longitude;
    m(1) = fl.latitude;
    m(2) = fl.rooms;
    m(3) = fl.price;
    m(4) = fl.footage;
    m(5) = fl.kitchen;
    m(6) = (fl.floor == 1 || fl.floor == fl.floors) ? 0 : -1; //  0 - если этаж первый или последний, в остальных случаях - 1
    return test(m);
}

void KMeans::serialize(const std::string& file_name) {
    dlib::serialize(file_name) << test;
}

void KMeans::deserialize(const std::string& file_name) {
    dlib::deserialize(file_name) >> test;
}
