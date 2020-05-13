#define BOOST_TEST_MODULE test_print

#include "print_ip.hpp"

#include <sstream>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_print)

BOOST_AUTO_TEST_CASE(test_valid_output)
{
    
    std::stringstream ss;
    print_ip(char(-1), ss);
	BOOST_TEST(ss.str() == "255");
    
    ss.str("");
    print_ip(short(0), ss);
    BOOST_TEST(ss.str() == "0.0");
    
    ss.str("");
    print_ip(int(2130706433), ss);
    BOOST_TEST(ss.str() == "127.0.0.1");
    
    ss.str("");
    print_ip(long(8875824491850138409), ss);
    BOOST_TEST(ss.str() == "123.45.67.89.101.112.131.41");

    std::string str = "24.56.78.54";
    ss.str("");
    print_ip(str, ss);
    BOOST_TEST(ss.str() == str);

    std::vector<int> vec = {14,18,123,255};
    ss.str("");
    print_ip(vec, ss);
    BOOST_TEST(ss.str() == "14.18.123.255");

    ss.str("");
    std::list<int> l = {14,18,123,255};
    print_ip(l, ss);
    BOOST_TEST(ss.str() == "14.18.123.255");
    
}

}
