//
//  console_writer.h
//  bulk
//
//  Created by a.polyakov on 02.06.2020.
//

#pragma once

#include "command_observer.h"

class ConsolePrinter : public CommandObserver {
public:
    virtual ~ConsolePrinter() {}
    
    void bulk_executed(const BulkResult& result) override;
    
protected:
    std::mutex console_mutex;
};
