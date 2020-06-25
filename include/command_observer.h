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

    
    virtual void bulk_executed(const BulkResult& /*result*/) {}
    virtual void command_process_stopped() {}
    virtual void command_read(const std::string& /*command_name*/) {}

protected:
	std::weak_ptr<CommandPublisher> publisher;
};
