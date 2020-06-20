#pragma once

#include "command_observer.h"

class FileWriter : public CommandObserver {
public:
	void bulk_executed(const CommandResult& result) override;
    
private:
    std::string create_filename() const;
};
