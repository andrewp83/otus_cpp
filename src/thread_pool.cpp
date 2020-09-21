#include "thread_pool.h"

namespace task_mgr {

ThreadPool::ThreadPool() {
    for (std::size_t i = 0; i < THREADS_COUNT; i++) {
        workers.emplace_back(*this);
    }
}

ThreadPool::~ThreadPool() {
    std::lock_guard<std::mutex> lock(queue_mutex); // Thread sanitizer warning fix
}


void ThreadPool::push_task(TaskPtr task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(task);
    }
    queue_cv.notify_one();
}

TaskPtr ThreadPool::pop_task() {
    //std::lock_guard<std::mutex> lock(queue_mutex);
    TaskPtr task = task_queue.front();
    task_queue.pop();
    return task;
}


}
