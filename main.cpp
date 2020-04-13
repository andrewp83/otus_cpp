#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "ip_filter.h"

int main(int argc, char const *argv[])
{
    try
    {
        std::vector<IpAddressV4> ip_pool;

        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.emplace_back(v.at(0));
        }

        // TODO reverse lexicographically sort
        std::sort(ip_pool.rbegin(), ip_pool.rend());
        print_ip_pool(ip_pool);

        auto filtered = ip_filter_1(ip_pool, 1);
        print_ip_pool(filtered);

        filtered = ip_filter_1_2(ip_pool, 46, 70);
        print_ip_pool(filtered);

        filtered = ip_filter_any(ip_pool, 46);
        print_ip_pool(filtered);
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
