#include <iostream>

#include "application.h"

void print_usage(std::ostream& out = std::cerr) {
    out << "Usage: bulk <commands number>" << std::endl;
}

int main (int argc, char ** argv) {
    
    if (argc != 2) {
        print_usage();
        return 0; // или не ноль?
    }
    
    size_t bulk_size = std::stoi(argv[1]);

    std::unique_ptr<Application> app = std::make_unique<Application>(bulk_size);
    app->run_main_loop();
    
    return 0;
}

