#include <iostream>
#include "matrix.hpp"
#include <array>

int main (int, char **) {
        
    Matrix<int, 0, 2> matrix;
    
    const size_t matrix_size = 10;
    for (size_t i = 0; i < matrix_size; i++) {
        matrix[ i ][ i ] = i;
        matrix[ i ][ matrix_size - i - 1 ] = matrix_size - i - 1;
    }

    for (size_t i = 1; i <= 8; i++) {
        for (size_t j = 1; j <= 8; j++) {
            if (j > 1) std::cout << " ";
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << matrix.size() << std::endl;
    
    for (const auto& c : matrix) {
        size_t i;
        size_t j;
        int v;
        std::tie(i, j, v) = c;
        std::cout << "[" << i << "][" << j << "] = " << v << std::endl;
    }
    
    return 0;
}

