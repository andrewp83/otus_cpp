#include <iostream>

#include "application.h"

namespace ui {
	void on_create() {
		Application::instance()->get_controller().create_new_document();
	}

	void on_load_from_file(const std::string& name) {
		Application::instance()->get_controller().open_document(name);
	}

	void on_save_to_file(const std::string& name, SaveDocumentFormat format) {
		Application::instance()->get_controller().save_document(name, format);
	}

	void on_shape_create(ShapeType type, Point pos) {
		Application::instance()->get_controller().create_shape(type, pos);
	}

	void on_shape_remove() {
		Application::instance()->get_controller().remove_shape();
	}

	void select_document(size_t index) {
		Application::instance()->get_controller().select_document(index);
	}
};

bool is_palindrom(const std::string& s, size_t beg) {
    size_t i_l = beg;
    size_t i_r = s.size() - 1;
    while (s[i_l] == s[i_r] && (i_r - i_l) > 1) {
        i_l++;
        i_r--;
    }
    return s[i_l] == s[i_r];
}

//size_t max_suffix_palindrom(const std::string& s) {
//    size_t i = 0;
//    while (!is_palindrom(s, i)) i++;
//    return i;
//}
//
//std::string palindrom(const std::string& a) {
//    if (std::all_of( a.begin(), a.end(), [=](const std::string::value_type& v) { return v == a[0]; })) {
//        return a + a[0];
//    }
//
//    std::string res = a;
//
//    size_t beg = max_suffix_palindrom(a);
//    if (beg == 0) {
//        auto it = a.rbegin();
//        std::advance(it, 1);
//        std::copy(it, a.rend(), std::back_inserter(res));
//    } else {
//        auto it = a.rbegin();
//        std::advance(it, a.size() - beg);
//        std::copy(it, a.rend(), std::back_inserter(res));
//    }
//
//    return res;
//}


std::string palindrom(const std::string& a) {
    std::string res = a;
    size_t i = 0;
    size_t pos = a.size();
    do {
        auto it = res.begin();
        std::advance(it, pos);
        res.insert(it, a[i++]);
    } while (!is_palindrom(res, 0));
    return res;
}


int main (int, char **) {
	Application::instance()->run();
    std::string pal = palindrom("abaabaaba");
    std::cout << pal << std::endl;
    
    pal = palindrom("a");
    std::cout << pal << std::endl;
    
    pal = palindrom("abababa");
    std::cout << pal << std::endl;
    
    pal = palindrom("qwerty");
    std::cout << pal << std::endl;
    
    return 0;
}
