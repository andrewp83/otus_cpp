#include "thread_worker.h"

#include <iostream>

#include "job.h"
#include "thread_pool.h"


namespace task_mgr {

ThreadWorker::ThreadWorker(ThreadPool& thread_pool)
: thread_pool(thread_pool) {
    t = std::thread(&ThreadWorker::run, this);
}

ThreadWorker::~ThreadWorker() {
    quit = true;
    thread_pool.get_queue_cv().notify_all();
    t.join();
}

void ThreadWorker::process_task(TaskPtr task) {
    task->run();
    task->get_job()->task_finished(task);
}

void ThreadWorker::run() {
    while (!quit) {
        std::unique_lock<std::mutex> lock(thread_pool.get_queue_mutex());
        thread_pool.get_queue_cv().wait(lock, [this]() {
            return !thread_pool.get_task_queue().empty() || quit;
        });
        if (!quit) {
            TaskPtr task = thread_pool.pop_task();
            lock.unlock();
            process_task(task);
        }
    }
    std::cout << "ThreadWorker::run" << std::endl;
}

}

