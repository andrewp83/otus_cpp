#pragma once

#include <iostream>
#include <thread>
#include <queue>
#include "common_types.h"

class ThreadManager;

class CommandWorker {
public:
    CommandWorker(ThreadManager& manager);
    virtual ~CommandWorker() {}
    
    void stop();
    void join();
    
    void push_command_result(const BulkResult& result);
    
    virtual void process_command_result(const BulkResult& result);
    
    const WorkerStats& get_stats() const { return stats; }
    
protected:
    void run();
    void log_command_result();
    
protected:
    ThreadManager& manager;
    std::thread t;
    
    bool is_stopped {false};
    
    std::queue<BulkResult> command_results;
    
    WorkerStats stats;
};


class LoggerWorker : public CommandWorker {
public:
    LoggerWorker(ThreadManager& manager, std::ostream& output = std::cout);
    
    void process_command_result(const BulkResult& result) override;
    
private:
    std::ostream& output;
};


class FileWorker : public CommandWorker {
public:
    FileWorker(ThreadManager& manager, std::size_t suffix);
    
    void process_command_result(const BulkResult& result) override;
    
private:
    std::string create_filename() const;
    std::size_t suffix;
};
