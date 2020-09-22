#pragma once

#include <list>
#include <mutex>
#include <queue>

#include "task.h"
#include "thread_worker.h"

namespace task_mgr {

class ThreadPool {
public:
    static const std::size_t DEFAULT_THREADS_COUNT;
    
public:
    ThreadPool(std::size_t threads_count = DEFAULT_THREADS_COUNT);
    ~ThreadPool() {}
    
    void stop();
    
    void push_task(TaskPtr task);
    TaskPtr pop_task();
    
    std::queue<TaskPtr>& get_task_queue() { return task_queue; }
    std::mutex& get_queue_mutex() { return queue_mutex; }
    std::condition_variable& get_queue_cv() { return queue_cv; }
    
private:
    std::list<ThreadWorker> workers;
    
    std::queue<TaskPtr> task_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
};

using ThreadPoolPtr = std::shared_ptr<ThreadPool>;


}
