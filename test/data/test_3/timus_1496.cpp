#include <algorithm>
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, size_t> accounts;

int main (int, char **) {
    size_t count;
    std::cin >> count;
    
    std::string name;
    for (size_t i = 0; i < count; i++) {
        std::cin >> name;
        auto it = accounts.find(name);
        if (it != accounts.end()) {
            it->second++;
        } else {
            accounts[name] = 1;
        }
    }
    
    for (const auto& _p : accounts) {
        if (_p.second > 1) {
            std::cout << _p.first << " " << _p.second << std::endl;
        }
    }
    
    return 0;
}

