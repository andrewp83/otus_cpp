#define BOOST_TEST_MODULE test_filter

#include "ip_filter.h"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_ip_addr)

BOOST_AUTO_TEST_CASE(test_valid_create)
{
    IpAddressV4 ip("123.43.45.91");
    int i = ip.getOctet(0);
    BOOST_CHECK(ip.getOctet(0) == 123);
    BOOST_CHECK(ip.getOctet(1) == 43);
    BOOST_CHECK(ip.getOctet(2) == 45);
    BOOST_CHECK(ip.getOctet(3) == 91);
}

BOOST_AUTO_TEST_CASE(test_valid_set)
{
    IpAddressV4 ip("123.43.45.91");
    
    ip.setOctet(0, 98);
    BOOST_CHECK(ip.getOctet(0) == 98);
    BOOST_CHECK(ip.getOctet(1) == 43);
    BOOST_CHECK(ip.getOctet(2) == 45);
    BOOST_CHECK(ip.getOctet(3) == 91);
    
    ip = IpAddressV4("123.43.45.91");
    ip.setOctet(1, 252);
    BOOST_CHECK(ip.getOctet(0) == 123);
    BOOST_CHECK(ip.getOctet(1) == 252);
    BOOST_CHECK(ip.getOctet(2) == 45);
    BOOST_CHECK(ip.getOctet(3) == 91);
    
    ip = IpAddressV4("123.43.45.91");
    ip.setOctet(2, 161);
    BOOST_CHECK(ip.getOctet(0) == 123);
    BOOST_CHECK(ip.getOctet(1) == 43);
    BOOST_CHECK(ip.getOctet(2) == 161);
    BOOST_CHECK(ip.getOctet(3) == 91);
    
    ip = IpAddressV4("123.43.45.91");
    ip.setOctet(3, 155);
    BOOST_CHECK(ip.getOctet(0) == 123);
    BOOST_CHECK(ip.getOctet(1) == 43);
    BOOST_CHECK(ip.getOctet(2) == 45);
    BOOST_CHECK(ip.getOctet(3) == 155);
}

}

const std::vector<std::string> test_data = {
    "113.162.145.156",
    "157.39.22.224",
    "79.180.73.190",
    "179.210.145.4",
    "219.102.120.135",
    "67.232.81.208",
    "85.254.10.197",
    "23.240.215.189",
    "1.29.168.152",
    "185.69.186.168",
    "110.152.103.161",
    "185.182.81.131",
    "68.45.152.8",
    "114.221.179.117",
    "185.89.100.249",
    "46.70.113.73",
    "188.68.0.38",
    "119.42.115.97",
    "194.106.108.214",
    "24.210.239.177",
};

std::vector<IpAddressV4> test_ip_pool;

static void testSetUp(const std::vector<std::string>& test_data) {
    test_ip_pool.clear();
    for (const auto& line : test_data) {
        std::vector<std::string> v = split(line, '\t');
        test_ip_pool.emplace_back(v.at(0));
    }
}

BOOST_AUTO_TEST_SUITE(test_filter)

BOOST_AUTO_TEST_CASE(test_valid_sort)
{
    testSetUp(test_data);

    // TODO reverse lexicographically sort
    std::sort(test_ip_pool.rbegin(), test_ip_pool.rend());
    IpAddressV4 prev(0, 0, 0, 0);
    for (auto it = test_ip_pool.cbegin(); it != test_ip_pool.cend(); ++it) {
        if (it != test_ip_pool.cbegin()) {
            BOOST_CHECK(*it < prev);
        }
        prev = *it;
    }
}

static void test_filter_1(const std::vector<std::string>& test_data) {
    testSetUp(test_data);
    std::sort(test_ip_pool.rbegin(), test_ip_pool.rend());
    auto filtered = ip_filter_1(test_ip_pool, 1);
    for (const auto& ip : filtered) {
        BOOST_CHECK(ip.getOctet(0) == 1);
    }
}

BOOST_AUTO_TEST_CASE(test_valid_filter_1)
{
    test_filter_1(test_data);
    test_filter_1({});
    test_filter_1({
        "1.2.3.4",
        "1.1.1.1"
    });
    test_filter_1({
        "8.8.8.8",
        "3.4.56.67",
        "78.34.90.133"
    });
}

BOOST_AUTO_TEST_CASE(test_valid_filter_1_2)
{
    testSetUp(test_data);

    std::sort(test_ip_pool.rbegin(), test_ip_pool.rend());
    auto filtered = ip_filter_1_2(test_ip_pool, 46, 70);
    for (const auto& ip : filtered) {
        BOOST_CHECK(ip.getOctet(0) == 46 && ip.getOctet(1) == 70);
    }
    print_ip_pool(filtered);
}

BOOST_AUTO_TEST_CASE(test_valid_filter_any)
{
    testSetUp(test_data);

    std::sort(test_ip_pool.rbegin(), test_ip_pool.rend());
    auto filtered = ip_filter_any(test_ip_pool, 46);
    for (const auto& ip : filtered) {
        BOOST_CHECK(ip.getOctet(0) == 46 || ip.getOctet(1) == 46 || ip.getOctet(2) ==46 || ip.getOctet(3) == 46 );
    }
}

}
