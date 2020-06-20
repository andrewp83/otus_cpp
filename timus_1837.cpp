#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
#include <queue>
#include <limits>

std::unordered_map<std::string, size_t> players_numbers;

std::unordered_map<std::string, std::unordered_set<std::string>> players_links;

std::vector<std::string> players;

std::queue<std::string> players_queue;

const size_t UNDEFINED_NUM = std::numeric_limits<size_t>::max();

int main (int, char **) {
    size_t count;
    std::cin >> count;
       
    // ТИПА СПИСКИ СМЕЖНОСТИ
    for (size_t i = 0; i < count; i++) {
        std::string p1, p2, p3;
        std::cin >> p1 >> p2 >> p3;
        players_links[p1].insert(p2);
        players_links[p1].insert(p3);
        players_links[p2].insert(p1);
        players_links[p2].insert(p3);
        players_links[p3].insert(p1);
        players_links[p3].insert(p2);
    }
    
    players.reserve(players_links.size());
    for (const auto& _p : players_links) {
        players.push_back(_p.first);
    }
    std::sort(players.begin(), players.end());
    
    // ТИПА BFS
    players_queue.push("Isenbaev");
    players_numbers["Isenbaev"] = 0;
    while (!players_queue.empty()) {
        const std::string& player = players_queue.front();
        players_queue.pop();
        size_t current_number = players_numbers[player] + 1;
        const std::unordered_set<std::string>& links = players_links[player];
        for (const auto& _p : links) {
            auto it = players_numbers.find(_p);
            if (it == players_numbers.end()) {
                it = players_numbers.insert(std::make_pair(_p, UNDEFINED_NUM)).first;
            }
            if (it->second > current_number) {
                it->second = current_number;
                players_queue.push(_p);
            }
        }
    }
    
    for (const auto& _p : players) {
        auto it = players_numbers.find(_p);
        const std::string& num_str = (it != players_numbers.end()) ? std::to_string(it->second) : "undefined";
        std::cout << _p << " " << num_str << std::endl;
    }
        
    return 0;
}

