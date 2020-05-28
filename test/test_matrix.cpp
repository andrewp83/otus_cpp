#include <sstream>

#include <gtest/gtest.h>

#include "matrix.hpp"


TEST(Matrix, Creation) {
    // Arrange
    Matrix<int, -1> matrix;
    
    // Act
    
    // Assert
    ASSERT_EQ(matrix.size(), 0);
}

TEST(Matrix, ReturnDefault) {
    Matrix<int, -1> matrix;
    auto a = matrix[0][0];
    ASSERT_EQ(a, -1);
    ASSERT_EQ(matrix.size(), 0);
}

TEST(Matrix, Assign) {
    Matrix<int, -1> matrix;
    matrix[100][100] = 314;
    ASSERT_EQ(matrix[100][100], 314);
}

TEST(Matrix, Iteration) {
    
    Matrix<int, -1> matrix;
    
    matrix[100][100] = 314;
    matrix[23][44] = 52;
    
    std::stringstream ss;
    for(auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        ss << x << y << v;
    }
    
    // РЕЗУЛЬТАТ ИМЕННО В ТАКОМ ПОРЯДКЕ ПОТОМУ ЧТО КЛЮЧИ В std::map ОТСОРТИРОВАНЫ
    ASSERT_EQ("234452100100314", ss.str());
}

TEST(Matrix, MultiAssign) {
    Matrix<int, -1> matrix;
    ((matrix[100][100] = 314) = 0) = 217;
    ASSERT_EQ(matrix[100][100], 217);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
