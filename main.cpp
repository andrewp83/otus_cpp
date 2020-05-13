#include "print_ip.hpp"

#include <list>
#include <vector>

int main(/*int argc, char const *argv[]*/)
{
    print_ip(char(-1));
    std::cout << std::endl;
    
    print_ip(short(0));
    std::cout << std::endl;
    
    print_ip(int(2130706433));
    std::cout << std::endl;
    
    print_ip(long(8875824491850138409));
    std::cout << std::endl;
    
    std::string str = "24.56.78.54";
    print_ip(str);
    std::cout << std::endl;
    
    std::vector<int> vec = {14,18,123,255};
    print_ip(vec);
    std::cout << std::endl;
    
    std::list<int> l = {14,18,123,255};
    print_ip(l);
    std::cout << std::endl;
}
