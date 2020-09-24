#pragma once

#include <list>
#include <mutex>
#include <queue>

#include "thread_worker.h"
#include "job.h"

namespace task_mgr {

class ThreadPool {
public:
    static const std::size_t DEFAULT_THREADS_COUNT;
    
public:
    ThreadPool(std::size_t threads_count = DEFAULT_THREADS_COUNT);
    ~ThreadPool() {}
    
    void stop();
    
    void push_task(JobTaskPtr task);
    JobTaskPtr pop_task();
    
    std::queue<JobTaskPtr>& get_task_queue() { return task_queue; }
    std::mutex& get_queue_mutex() { return queue_mutex; }
    std::condition_variable& get_queue_cv() { return queue_cv; }
    
private:
    std::list<ThreadWorker> workers;
    
    std::queue<JobTaskPtr> task_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
};

using ThreadPoolPtr = std::shared_ptr<ThreadPool>;


}
