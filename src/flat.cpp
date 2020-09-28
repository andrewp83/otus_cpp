#include "flat.h"


void flat_from_str(const std::string& str, Flat& flat, unsigned long& cluster) {
    
    auto str_to_d = [](const std::string& str)->double {
        if (str.empty()) return 0.0;
        return std::stod(str);
    };
    
    std::vector<std::string> v = split(str, ';');
    
    assert(v.size() >= 8);
    
    flat.longitude = str_to_d(v.at(0));
    flat.latitude = str_to_d(v.at(1));
    flat.rooms = str_to_d(v.at(2));
    flat.price = str_to_d(v.at(3));
    flat.footage = str_to_d(v.at(4));
    flat.kitchen = str_to_d(v.at(5));
    flat.floor = str_to_d(v.at(6));
    flat.floors = str_to_d(v.at(7));
    if (v.size() > 8) {
        cluster = std::stoi(v.at(8));
    }
}
