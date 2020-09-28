#include "k_means.h"

#include <cassert>

#include "utils.h"

KMeans::KMeans(unsigned long num_clusters, double kernel_gamma, double tolerance, unsigned long max_dictionary_size)
: num_clusters(num_clusters)
, kc(kernel_type(kernel_gamma), tolerance, max_dictionary_size)
, test(kc) {
    
}

KMeans::KMeans()
: test(kc) {
    
}

void KMeans::serialize(const std::string& file_name) {
    dlib::serialize(file_name + ".dat") << test;
    std::ofstream fs;
    fs.open(file_name + ".mm.csv");
    std::string min_max_str;
    for(std::size_t i = 0; i < sample_type::NR; i++) {
        min_max_str += std::to_string(rows_min_values[i]);
        min_max_str += ";";
        min_max_str += std::to_string(rows_max_values[i]);
        if (i != (sample_type::NR - 1)) {
            min_max_str += ";";
        }
    }
    fs << min_max_str;
    fs << std::endl;
}

void KMeans::deserialize(const std::string& file_name) {
    dlib::deserialize(file_name + ".dat") >> test;
    std::ifstream fs;
    fs.open(file_name + ".mm.csv");
    rows_min_values.clear();
    rows_max_values.clear();
    for(std::string line; std::getline(fs, line);) {
        std::vector<std::string> str_values = split(line, ';');
        assert(str_values.size() == sample_type::NR * 2);
        for(std::size_t i = 0; i < sample_type::NR; i++) {
            rows_min_values[i] = std::stod(str_values[i * 2]);
            rows_max_values[i] = std::stod(str_values[i * 2 + 1]);
        }
    }
}

void KMeans::update_min_max(long index, sample_type::type value) {
    auto it = rows_min_values.find(index);
    if (it == rows_min_values.end()) {
        rows_min_values.emplace(index, value);
    } else {
        it->second = std::min(it->second, value);
    }
    
    it = rows_max_values.find(index);
    if (it == rows_max_values.end()) {
        rows_max_values.emplace(index, value);
    } else {
        it->second = std::max(it->second, value);
    }
}

void KMeans::normalize_sample(sample_type& sample) const {
    for (long i = 0; i < sample_type::NR; i++) {
        auto it_min = rows_min_values.find(i);
        auto it_max = rows_max_values.find(i);
        assert(it_min != rows_min_values.end());
        assert(it_max != rows_max_values.end());
        sample(i) = (sample(i) - it_min->second) / (it_max->second - it_min->second) * 100.0;
    }
}
