#include "thread_manager.h"

#include <iostream>

ThreadManager::ThreadManager(std::ostream& output)
: output(output) {
    
    log_worker = std::make_unique<LoggerWorker>(*this, output);
    
    file_workers.push_back(std::make_unique<FileWorker>(*this, 1));
    file_workers.push_back(std::make_unique<FileWorker>(*this, 2));
}

void ThreadManager::bulk_executed(const BulkResult& result) {
    // пробудить воркеров
    {
        std::lock_guard<std::mutex> lk(m);
        
        auto push_result = [&](std::unique_ptr<CommandWorker>& worker) {
            worker->push_command_result(result);
        };
        
        push_result(log_worker);

        if (!file_workers.empty()) {
            push_result(file_workers[current_file_worker]);
            current_file_worker = (current_file_worker + 1) % file_workers.size();
        }
    }
    
    cv.notify_all();
}

void ThreadManager::command_process_stopped() {
    stop_all();
}

std::condition_variable& ThreadManager::get_cv() {
    return cv;
}

std::mutex& ThreadManager::get_mutex() {
    return m;
}

void ThreadManager::stop_all() {
    
    auto stop = [](std::unique_ptr<CommandWorker>& worker) {
        worker->stop();
    };
    stop(log_worker);
    std::for_each(file_workers.begin(),
                  file_workers.end(),
                  stop);
    
    cv.notify_all();
    
    auto join = [](std::unique_ptr<CommandWorker>& worker) {
        worker->join();
    };
    join(log_worker);
    std::for_each(file_workers.begin(),
                  file_workers.end(),
                  join);
}

void ThreadManager::print_stats() {
    
    // log
    output << "log thread: " << log_worker->get_stats().to_string() << std::endl;
    
    // files
    for (size_t i = 0; i < file_workers.size(); i++) {
        const std::unique_ptr<CommandWorker>& worker = file_workers[i];
        output << "file1 thread_" << std::to_string(i + 1) << ": " << worker->get_stats().to_string() << std::endl;
    }
}
