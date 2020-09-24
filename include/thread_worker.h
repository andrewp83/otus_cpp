#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "job.h"

namespace task_mgr {

class ThreadPool;

class ThreadWorker {
public:
    ThreadWorker(ThreadPool& thread_pool);
    ~ThreadWorker();
    
    void run();
    
private:
    void process_task(JobTaskPtr task);

private:
    std::thread t;
    std::atomic<bool> quit {false};
    ThreadPool& thread_pool;
};



}
