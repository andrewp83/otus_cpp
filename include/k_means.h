#pragma once

#include <vector>
#include <unordered_map>

#include <dlib/clustering.h>


using DataRow = std::vector<double>;

class KMeans {
public:
    KMeans(unsigned long num_clusters, double kernel_gamma, double tolerance, unsigned long max_dictionary_size = 1000000);
    KMeans();
    
    template<class T>
    void fit(const T& data) {
        
        std::vector<sample_type> samples;
        sample_type m;
        
        rows_min_values.clear();
        rows_max_values.clear();
        
        for (const auto& row : data) {
            for (std::size_t index = 0; index < row.size(); ++index) {
                double value = row[index];
                m(index) = value;
                update_min_max(index, value);
            }
            samples.push_back(m);
        }
        
        for (auto& m : samples) {
            normalize_sample(m);
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
    
    template<class T>
    unsigned long predict(const T& data_row) const {
        sample_type m;
        for (std::size_t index = 0; index < data_row.size(); index++) {
            m(index) = data_row[index];
        }
        normalize_sample(m);
        return test(m);
    }
    
    void serialize(const std::string& file_name);
    void deserialize(const std::string& file_name);
    
private:
    typedef dlib::matrix<double, 7, 1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;
    
private:
    
    void update_min_max(long index, sample_type::type value);
    
    void normalize_sample(sample_type& sample) const;

    unsigned long num_clusters {0};
    
    dlib::kcentroid<kernel_type> kc;
    dlib::kkmeans<kernel_type> test;
    
    std::unordered_map<long, sample_type::type> rows_min_values;
    std::unordered_map<long, sample_type::type> rows_max_values;
};
