#include "ip_filter.h"

#include <iostream>


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

template<class Pred>
IpPool ip_filter(const IpPool& pool, Pred pred) {
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), pred);
    return result;
}

IpPool ip_filter_1(const IpPool& pool, octet_t value) {
    IpAddressV4 ip(value, 0, 0, 0);
    auto comp = [](const IpAddressV4& left, const IpAddressV4& right) {
        return left.getOctet(0) > right.getOctet(0);
    };
    auto eqR = std::equal_range(pool.cbegin(), pool.cend(), ip, comp);
    IpPool result;
    std::copy(eqR.first, eqR.second, std::back_inserter(result));
    return result;
}

IpPool ip_filter_1_2(const IpPool& pool, octet_t value1, octet_t value2) {
    IpAddressV4 ip(value1, value2, 0, 0);
    auto comp = [](const IpAddressV4& left, const IpAddressV4& right) {
        return ((left.getOctet(0) << 8) + left.getOctet(1)) > ((right.getOctet(0) << 8) + right.getOctet(1));
    };
    auto eqR = std::equal_range(pool.cbegin(), pool.cend(), ip, comp);
    IpPool result;
    std::copy(eqR.first, eqR.second, std::back_inserter(result));
    return result;
}

IpPool ip_filter_any(const IpPool& pool, octet_t value) {
    // наверное можно сделать лучше
    return ip_filter(pool, [=](const IpAddressV4& ip) {
        bool found = false;
        for (auto i = 0; i < ip.LENGTH; i++) {
            if (ip.getOctet(i) == 46) {
                found = true;
                break;
            }
        }
        return found;
    });
}

void print_ip_pool(const IpPool& pool) {
    for(const auto& ip : pool) {
        std::cout << ip.toString();
        std::cout << std::endl;
    }
}
