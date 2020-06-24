//
//  common_types.h
//  bulk
//
//  Created by a.polyakov on 02.06.2020.
//

#pragma once

#include <string>
#include <list>

using CommandResult = std::string;

struct BulkResult {
    std::list<CommandResult> command_results;
    
    std::string to_string() const {
        std::string result;
        for (const auto& cmd_res : command_results) {
            if (!result.empty()) result += ", ";
            result += cmd_res;
        }
        return "bulk: " + result;
    }    
};

struct WorkerStats {
    size_t bulks_count {0};
    size_t commands_count {0};
};
