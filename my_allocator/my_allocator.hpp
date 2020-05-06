#include <iostream>
#include <limits>
#include <map>
#include <vector>

#include "memory_mgr.h"

#ifdef __linux__ 

#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define WHITE   "\033[37m"      /* White */

#else

#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define WHITE   ""

#endif

#ifdef _WIN64

#define __PRETTY_FUNCTION__ __FUNCDNAME__

#endif

template<typename T, size_t BlockSize>
struct my_allocator {
    
    using value_type = T;

    // не обязательно для GCC
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    // не обязательно для GCC, MSVC
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = my_allocator<U, BlockSize>;
    };

    my_allocator() = default;
    ~my_allocator() = default;

    template<typename U> 
    my_allocator(const my_allocator<U, BlockSize>&) {}

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    T *allocate(std::size_t n) {
        //std::cout << GREEN << __PRETTY_FUNCTION__ << " [n = " << n << "]" 
               // << WHITE << std::endl;
        //auto p = std::malloc(n * sizeof(T));
        auto p = MemoryMgr::get_instance(BlockSize).memory_alloc(n * sizeof(T));
        if (!p) {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T *>(p);
    }

    void deallocate(T *p, std::size_t n) {
        //std::cout << RED << __PRETTY_FUNCTION__ << " [n  = " << n << "] "
               //  << WHITE << std::endl;
        //std::free(p);
        MemoryMgr::get_instance(BlockSize).memory_free(p);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        //std::cout << BLUE << __PRETTY_FUNCTION__ << WHITE << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };

    template<typename U>
    void destroy(U *p) {
        //std::cout << YELLOW << __PRETTY_FUNCTION__ << WHITE << std::endl;
        p->~U();
    }

    // а для соответствия стандарту еще нужно
    // 1. функция address
    // 2. операторы сравнения == и !=
    
    const_pointer address( const_reference x ) const {
        return &x;
    }
};

template<typename U1, size_t BlockSize1, typename U2, int BlockSize2>
bool operator==(const my_allocator<U1, BlockSize1>& lhs, const my_allocator<U2, BlockSize2>& rhs) {
    return BlockSize1 == BlockSize2;
}

template<typename U1, size_t BlockSize1, typename U2, int BlockSize2>
bool operator!=(const my_allocator<U1, BlockSize1>& lhs, const my_allocator<U2, BlockSize2>& rhs) {
   return !(lhs == rhs);
}

// int main(int, char *[]) {
//     auto v = std::vector<int, my_allocator<int>>{};
//     // v.reserve(6);
//     for (int i = 0; i < 6; ++i) {
//         std::cout << "vector size = " << v.size() << std::endl;
//         v.emplace_back(i); // std::forward ссылка
//         std::cout << std::endl;
//     }

//     /* auto m = std::map<int, int, std::less<int>, 
//        my_allocator<std::pair<const int, int>>>{};
//     for (int i = 0; i < 3; ++i) {
//        m[i] = i;
//        std::cout << std::endl;
//     }*/

//     return 0;
// }
