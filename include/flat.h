//
//  flat.h
//  kkmeans

#pragma once

#include "utils.h"


struct Flat {
    double longitude {0.0};
    double latitude {0.0};
    int rooms {0};
    double price {0.0};
    double footage {0.0};
    double kitchen {0.0};
    int floor {0};
    int floors {0};
};

void flat_from_str(const std::string& str, Flat& flat, unsigned long& cluster);
