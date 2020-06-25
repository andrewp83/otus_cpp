#include "stats_manager.h"

void StatsManager::print_stats(std::ostream& output) {
    output << "main: " << std::to_string(lines_read_count) << " lines, " << stats.to_string() << std::endl;
}

void StatsManager::bulk_executed(const BulkResult& result) {
    stats.bulks_count++;
    stats.commands_count += result.command_results.size();
}

void StatsManager::command_read(const std::string& /*command_name*/) {
    lines_read_count++;
}

