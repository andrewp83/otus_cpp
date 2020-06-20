#include <algorithm>
#include <iostream>
#include <vector>

int main (int, char **) {
    uint32_t n;
    std::cin >> n;
    
    std::vector<uint32_t> teacher_dates(n);
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> teacher_dates[i];
    }
    
    std::cin >> n;
    uint32_t matched_dates = 0;
    uint32_t student_date;
    for (uint32_t i = 0; i < n; i++) {
        std::cin >> student_date;
        if (std::binary_search(teacher_dates.cbegin(), teacher_dates.cend(), student_date)) {
            matched_dates++;
        }
    }
    
    std::cout << matched_dates << std::endl;
    
    return 0;
}

