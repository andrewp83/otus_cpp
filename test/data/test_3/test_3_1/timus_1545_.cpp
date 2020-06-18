#include <algorithm>
#include <array>
#include <iostream>
#include <list>

std::array<std::list<char>, 26> all_tips;

int main (int, char **) {
    size_t count;
    std::cin >> count;
    
    auto char_to_idx = [](char ch)->size_t {
        return ch - 'a';
    };
    
    std::array<char, 2> glyph;
    for (size_t i = 0; i < count; i++) {
        std::cin >> glyph[0] >> glyph[1];
        size_t i_tip = char_to_idx(glyph[0]);
        all_tips[i_tip].push_back(glyph[1]);
    }
    
    char letter;
    std::cin >> letter;
    size_t i_tip = char_to_idx(letter);
    auto& tips = all_tips[i_tip];
    for_each(tips.cbegin(), tips.cend(), [letter](char ch){
        std::cout << letter << ch << std::endl;
    });
    
    return 0;
}

