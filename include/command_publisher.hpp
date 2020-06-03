#pragma once

#include <list>
#include <set>

class CommandObserver;

class CommandPublisher {
public:
	void subscribe(CommandObserver* obj) {
		auto it = std::find(observers.begin(), observers.end(), obj);
		if (it == observers.end()) {
			observers.push_back(obj);
		}
	}

	void unsubscribe(CommandObserver* obj) {
        removes.insert(obj);
        remove_observers();
	}
    
    void remove_observers() {
        if (is_in_notify) return;
        
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

	template<typename Func, typename... Args>
	void notify(Func func, Args... args) {
        is_in_notify = true;
        
		for(auto obs : observers){
			(obs->*func)(args...);
		};
        
        is_in_notify = false;
        remove_observers();
	}

private:
	std::list<CommandObserver*> observers;
    std::set<CommandObserver*> removes;
    bool is_in_notify {false};
};
