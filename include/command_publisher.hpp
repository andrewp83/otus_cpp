#pragma once

#include <list>

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
		auto it = std::find(observers.begin(), observers.end(), obj);
		if (it != observers.end()) {
			observers.erase(it);
		}
	}

	template<typename Func, typename... Args>
	void notify(Func func, Args... args) {
		for(auto obs : observers){
			(obs->*func)(args...);
		};
	}

private:
	std::list<CommandObserver*> observers;
};
