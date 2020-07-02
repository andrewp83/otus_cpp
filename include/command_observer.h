#pragma once

#include <memory>
#include "common_types.h"

class CommandObserver {
public:
    virtual void bulk_executed(const BulkResult& /*result*/) {}
    virtual void command_process_stopped() {}
    virtual void command_read(const std::string& /*command_name*/) {}
};
