#pragma once

#include "command_observer.h"

#include <mutex>

class FileWriter : public CommandObserver {
public:
    virtual ~FileWriter() {}
    
	void bulk_executed(const BulkResult& result) override;
    
private:
    std::string create_filename();
    
    std::mutex log_mutex;
    
    std::time_t last_time {0};
    std::size_t add_suffix {0};
};
