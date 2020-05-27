#include <iostream>
//#include "src/matrix.cpp"
#include <math.h>
#include <set>

int main (int, char **) {
    
    //Application::instance().run();
    

    int n;
    std::cin >> n;
    std::set<std::pair<int, int>> coords;
    int x, y;
    int x_min = 0, x_max = 0;
    int y_min = 0, y_max = 0;
    while (n--) {
        std::cin >> x >> y;
        coords.emplace(x, y);
        x_min = std::min(x_min, x);
        x_max = std::max(x_max, x);
        y_min = std::min(y_min, y);
        y_max = std::max(y_max, y);
    }
    
    for(int j = y_max;  j >= y_min; j--) {
        for (int i = x_min; i <= x_max; i++) {
            if (coords.count(std::make_pair(i, j)) > 0) {
                std::cout << "*";
            } else if (i == 0 && j == 0) {
                std::cout << "+";
            } else if (i == 0) {
                std::cout << "|";
            } else if (j == 0) {
                std::cout << "-";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    
//    Series<int, -1> s;
//    ((s[2] = 42) = 0) = 217;
//    //int(s[2]) += 10;
//    std::cout << s[2] << s[42] << std::endl;
//    std::cout << s.size() << std::endl;
    
    return 0;
}

