#include <iostream>
#include <limits.h>
#include <type_traits>
#include <tuple>

static const char* DOT_SEPARATOR = ".";

template<typename T>
typename std::enable_if_t<std::is_integral<T>::value> print_ip(const T& addr, std::ostream& output = std::cout) {
    // TO DO
    //output << __PRETTY_FUNCTION__ << std::endl;
    size_t num_bytes = sizeof(T);
    size_t n = num_bytes;
    while (n) {
        if (n < num_bytes) {
            output << DOT_SEPARATOR;
        }
        uint8_t byte = (addr >> (8 * (n - 1))) & 0xff;
        output << static_cast<short>(byte);
        n--;
    }
}

// std::string
template<typename T>
typename std::enable_if_t<std::is_same<T, std::string>::value> print_ip(const T& addr, std::ostream& output = std::cout) {
    output << addr;
}

// containers
template<typename T>
//decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()), void()) print_ip(const T& addr) {
typename std::enable_if_t< !std::is_same<T, std::string>::value
                        && std::is_same<typename T::iterator, typename T::iterator>::value > print_ip(const T& addr, std::ostream& output = std::cout) {
                            
    for (auto it = addr.begin(); it != addr.end(); ++it) {
        if (it != addr.begin()) output << DOT_SEPARATOR;
        output << *it;
    }
}

template<typename T, size_t N>
struct tuple_printer : tuple_printer<T, N - 1> {
    using base = tuple_printer<T, N - 1>;
        
    void print(const T& t, std::ostream& output = std::cout) {
        base::print(t);
       
        output << std::get<N - 1>(t);
        
//        if (N > 1) {
//            static_assert(std::is_same<decltype(std::get<N - 1>(t)), decltype(std::get<N - 2>(t))>::value, "types not equal");
//        }
        
        if (std::tuple_size<T>::value != N) {
            output << ".";
        }
    }
};

template<typename T>
struct tuple_printer<T, 0> {
    void print(const T&) {
    }
};

template<typename T, size_t n = std::tuple_size<T>::value>
decltype(std::get<0>(std::declval<T>()), void()) print_ip(const T& addr, std::ostream& output = std::cout) {
    tuple_printer<T, n> t_pr;
    t_pr.print(addr, output);
}
