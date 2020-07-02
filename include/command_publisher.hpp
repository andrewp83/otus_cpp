#pragma once

#include <list>
#include <set>
#include <memory>

#include "command_observer.h"

template<typename T>
class Publisher {
public:
    
    static void add(const std::shared_ptr<T>& obs) {
        observers.push_back(obs);
    }
    
    template<typename Func, typename... Args>
    static void notify(Func func, Args... args) {
        
        auto it = observers.begin();
        
        while (it != observers.end()){
            auto observer = it->lock();
            if (observer) {
                ((*observer).*func)(args...);
                it++;
            } else {
                it = observers.erase(it);
            }
        };
        
    }
    
private:
    static std::list<std::weak_ptr<T>> observers;
};

template<typename T>
std::list<std::weak_ptr<T>> Publisher<T>::observers;
