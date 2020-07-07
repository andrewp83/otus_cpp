#include <algorithm>
#include <iostream>
#include "stdio.h"
#include <vector>

int main (int, char **) {
    uint32_t n;
    scanf("%d", &n);
    
    std::vector<uint32_t> teacher_dates(n);
    for (uint32_t i = 0; i < n; i++) {
        scanf("%d", &(teacher_dates[i]));
    }
    
    scanf("%d", &n);
    uint32_t matched_dates = 0;
    uint32_t student_date;
    for (uint32_t i = 0; i < n; i++) {
        scanf("%d", &student_date);
        if (std::binary_search(teacher_dates.cbegin(), teacher_dates.cend(), student_date)) {
            matched_dates++;
        }
    }
    
    printf("%d\n", matched_dates);
    
    return 0;
}

