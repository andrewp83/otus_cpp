#include <iostream>
#include <map>

#include "my_allocator.hpp"
#include "my_list.hpp"

static const size_t ALLOCATED_BLOCK_SIZE = 200;

int main(int argc, char const *argv[])
{
    auto factorial = [](int num)->int {
        int fact = 1;
        for (int i = 2; i <= num; i++) {
            fact *= i;
        }
        return fact;
    };

    std::map<int, int> test_map;
    for (auto i = 0; i < 10; i++) {
        test_map.insert(std::make_pair(i, factorial(i)));
    }
    
    std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>, ALLOCATED_BLOCK_SIZE>> test_alloc_map;
    for (auto i = 0; i < 10; i++) {
        test_alloc_map.insert(std::make_pair(i, factorial(i)));
    }
    
    std::for_each(test_alloc_map.cbegin(), test_alloc_map.cend(), [](const decltype(test_alloc_map)::value_type& value){
        std::cout << value.first << " " << value.second << std::endl;
    });
    
    my_list<int> test_list;
    for (auto i = 0; i < 10; i++) {
        test_list.push_back(i);
    }
    
    my_list<int, my_allocator<int, ALLOCATED_BLOCK_SIZE>> test_alloc_list;
    for (auto i = 0; i < 10; i++) {
        test_alloc_list.push_back(i);
    }
    
    std::for_each(std::begin(test_alloc_list), std::end(test_alloc_list), [](const decltype(test_alloc_list)::value_type& value){
        std::cout << value << std::endl;
    });
    
    return 0;
}
