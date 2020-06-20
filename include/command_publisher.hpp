#pragma once

#include <list>
#include <set>

#include "command_observer.h"

//class CommandObserver;

class CommandPublisher {
public:
    
	void add_observer(CommandObserver* obj) {
        observers.push_back(obj);
	}

	void remove_observer(CommandObserver* obj) {
        removes.insert(obj);
        clear_observers();
	}
    
    void clear_observers() {
        if (observers_locked) return;
        
        auto it = observers.begin();
        while (it != observers.end()) {
            if (removes.count(*it) > 0) {
                it = observers.erase(it);
            } else {
                ++it;
            }
        }
        removes.clear();
    }
    
    size_t observers_count() const {
        return observers.size();
    }

	template<typename Func, typename... Args>
	void notify(Func func, Args... args) {
        observers_locked = true;
        
		for(auto obs : observers){
			(obs->*func)(args...);
		};
        
        observers_locked = false;

        clear_observers();
	}

private:
	std::list<CommandObserver*> observers;
    std::set<CommandObserver*> removes;
    bool observers_locked {false};
};
