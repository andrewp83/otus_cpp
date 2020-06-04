#include "command_observer.h"
#include "command_publisher.hpp"

void CommandObserver::subscribe(std::shared_ptr<CommandPublisher> publisher) {
    unsubscribe();

	publisher->add_observer(this);
    
	this->publisher = publisher;
}

void CommandObserver::unsubscribe() {
	if (auto ptr = publisher.lock()) {
		ptr->remove_observer(this);
	}
}
