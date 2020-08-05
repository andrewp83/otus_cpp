#pragma once

#include <list>
#include <set>
#include <memory>

template<typename T>
class Publisher {
public:
    
    void add(const std::shared_ptr<T>& obs) {
        observers.push_back(obs);
    }
    
    template<typename Func, typename... Args>
    void notify(Func func, Args... args) {
        
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
    std::list<std::weak_ptr<T>> observers;
};
