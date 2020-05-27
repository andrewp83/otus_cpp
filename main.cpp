#include <iostream>
#include "src/matrix.cpp"
#include <array>

int main (int, char **) {
    
    //Matrix<int, -1, 2> m;
    //Matrix<int, -1> m;
    
    //std::vector<int, 3> a = {3,4,5};
    //int x , y, z;
    //std::tie(x, y, z) = a;
    //std::cout << x << y << z;
    
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    
    for(auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
    
    return 0;
}

