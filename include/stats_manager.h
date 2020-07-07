#pragma once

#include <iostream>

#include "common_types.h"
#include "command_observer.h"

class StatsManager : public CommandObserver {
public:
    void print_stats(std::ostream& output = std::cout);
    
    void bulk_executed(const BulkResult& /*result*/) override;
    void command_read(const std::string& /*command_name*/) override;
    
protected:
    size_t lines_read_count {0};
    WorkerStats stats;
};
