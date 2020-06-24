#include "thread_manager.h"

#include <iostream>

ThreadManager::ThreadManager() {
    log_worker = std::make_unique<LoggerWorker>(*this);
    
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
            if (!(current_file_worker >= 0 && current_file_worker < file_workers.size())) {
                current_file_worker = 0;
            }
            push_result(file_workers[current_file_worker]);
            current_file_worker++;
        }
    }
    
    cv.notify_all();
}

void ThreadManager::command_process_stopped() {
    stop_all();
    print_stats();
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
    std::cout << "ThreadManager::print_stats()" << std::endl;
}
