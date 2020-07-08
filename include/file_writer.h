#pragma once

#include "command_observer.h"

class FileWriter : public CommandObserver {
public:
    virtual ~FileWriter() {}
    
	void bulk_executed(const BulkResult& result) override;
    
private:
    std::string create_filename() const;
};
