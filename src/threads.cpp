#include "threads.h"

#include <iostream>

#include "thread_manager.h"

CommandWorker::CommandWorker(ThreadManager& manager)
: manager(manager) {
    t = std::thread(&CommandWorker::run, this);
}

void CommandWorker::push_command_result(const BulkResult &result) {
    command_results.push(result);
}

void CommandWorker::process_command_result(const BulkResult& result) {
    stats.commands_count += result.command_results.size();
    stats.bulks_count++;
}

void CommandWorker::run() {
    while (!is_stopped) {
        std::unique_lock<std::mutex> lk(manager.get_mutex());
        manager.get_cv().wait(lk, [this]{ return !command_results.empty() || is_stopped; });
        log_command_result();
    }
}

void CommandWorker::log_command_result() {
    if (command_results.empty()) {
        return;
    }
    const BulkResult& result = command_results.front();
    process_command_result(result);
    command_results.pop();
}

void CommandWorker::join() {
    t.join();
}

void CommandWorker::stop() {
    is_stopped = true;
}


// LoggerWorker

LoggerWorker::LoggerWorker(ThreadManager& manager, std::ostream& output)
: CommandWorker(manager)
, output(output) {
    
}

void LoggerWorker::process_command_result(const BulkResult& result) {
    
    CommandWorker::process_command_result(result);
    
    output << result.to_string() << std::endl;
}


// FileWorker

#include <fstream>

FileWorker::FileWorker(ThreadManager& manager, std::size_t suffix)
: CommandWorker(manager)
, suffix(suffix) {
    
}

void FileWorker::process_command_result(const BulkResult& result) {
    
    CommandWorker::process_command_result(result);
    
    // save to file
    const std::string& filename = create_filename();
    std::fstream fs(filename, fs.out);
    if (!fs.is_open()) {
        std::cerr << "failed to open " << filename << std::endl;
        return;
    }
    fs << result.to_string();
}

std::string FileWorker::create_filename() const {
    return "bulk" + std::to_string(time(nullptr)) + "_" + std::to_string(suffix) + ".log";
}
