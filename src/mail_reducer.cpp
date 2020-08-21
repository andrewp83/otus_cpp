#include "mail_reducer.h"

#include <iostream>

void MailReducer::call(const MailContainer& chunk) {
    std::lock_guard<std::mutex> l(out_m);
    std::cout << "reduce size: " << chunk.size() << std::endl;
}
