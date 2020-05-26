#include <iostream>

void decrypt(std::string& s, size_t beg, size_t end) {
    size_t len = end - beg + 1;
    if (len < 3) return;
    size_t mid = beg + (len + 1) / 2 - 1;
    char ch = s[beg];
    s.insert(mid + 1, 1, ch);
    //s.erase(mid, 1);
    //s.insert(0, 1, ch);
    s.erase(beg, 1);
    decrypt(s, beg, mid - 1);
    decrypt(s, mid + 1, end);
}

int main (int, char **) {
    std::string s;
    std::getline(std::cin, s);
    decrypt(s, 0, s.size() - 1);
    std::cout << s << std::endl;
    return 0;
}

//int main (int, char **) {
//
//	//Application::instance()->run();
//
//    long long int i, Fi, j, Fj, n;
//    std::cin >> i >> Fi >> j >> Fj >> n;
//
//    if (j < i) {
//        std::swap(i, j);
//        std::swap(Fi, Fj);
//    }
//
//    long long int F_i_plus_1 = 0;
//    if (j > i + 1) {
//        long long int x = 1, y = 1;
//        long long int cur = i + 2;
//        long long int x_prev;
//        while (cur < j) {
//            x_prev = x;
//            x = x + y;
//            y = x_prev;
//            cur++;
//        }
//        F_i_plus_1 = (Fj - y * Fi) / x;
//        //std::cout << x << ", " << y << std::endl;
//    } else {
//        F_i_plus_1 = j;
//    }
//
//    long long int Fn = 0;
//    if (n == i) {
//        Fn = Fi;
//    } else if (n == j) {
//        Fn = Fj;
//    } else if (n == i + 1) {
//        Fn = F_i_plus_1;
//    } else {
//        if (n > i) {
//            long long int cur = i + 2;
//            while (cur <= n) {
//                Fn = F_i_plus_1 + Fi;
//                Fi = F_i_plus_1;
//                F_i_plus_1 = Fn;
//                cur++;
//            }
//        } else {
//            long long int cur = i - 1;
//            while (cur >= n) {
//                Fn = F_i_plus_1 - Fi;
//                F_i_plus_1 = Fi;
//                Fi = Fn;
//                cur--;
//            }
//        }
//    }
//
//    std::cout << Fn << std::endl;
//
//    return 0;
//}
