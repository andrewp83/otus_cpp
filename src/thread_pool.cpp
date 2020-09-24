#include "thread_pool.h"

namespace task_mgr {

const std::size_t ThreadPool::DEFAULT_THREADS_COUNT = 10;

ThreadPool::ThreadPool(std::size_t threads_count) {
    for (std::size_t i = 0; i < threads_count; i++) {
        workers.emplace_back(*this);
    }
}

void ThreadPool::stop() {
    workers.clear();
}

void ThreadPool::push_task(JobTaskPtr task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(task);
    }
    queue_cv.notify_one();
}

JobTaskPtr ThreadPool::pop_task() {
    //std::lock_guard<std::mutex> lock(queue_mutex);
    JobTaskPtr task = task_queue.front();
    task_queue.pop();
    return task;
}


}
