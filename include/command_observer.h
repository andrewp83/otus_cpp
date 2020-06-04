#pragma once

#include <memory>
#include "common_types.h"

class CommandPublisher;

class CommandObserver {
public:
	virtual ~CommandObserver() {
		unsubscribe();
	}

	void subscribe(std::shared_ptr<CommandPublisher> publisher);
	void unsubscribe();

	virtual void bulk_executed(const CommandResult& result) = 0;

protected:
	std::weak_ptr<CommandPublisher> publisher;
};
