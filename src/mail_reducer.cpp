#include "mail_reducer.h"

#include <iostream>

bool MailReducer::call(const MailContainer& chunk) {
    if (chunk.size() < 2) {
        return true;
    }
    
    bool all_different = true;
    auto it = chunk.begin();
    it++;
    while (it != chunk.end() && all_different) {
        if (*it == *(std::prev(it))) all_different = false;
        it++;
    }
    
    return all_different;
}
