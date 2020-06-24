#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "common_types.h"
#include "command_observer.h"
#include "threads.h"


class ThreadManager : public CommandObserver {
public:
    ThreadManager();
    
    // CommandObserver
    void bulk_executed(const BulkResult& result) override;
    void command_process_stopped() override;
    
    std::condition_variable& get_cv();
    std::mutex& get_mutex();
    
private:
    void stop_all();
    void print_stats();
    
private:
    std::unique_ptr<CommandWorker> log_worker;
    std::vector<std::unique_ptr<CommandWorker>> file_workers;
    std::size_t current_file_worker {0};
    
    std::condition_variable cv;
    std::mutex m;
};
