//#include "thread_worker.h"
//
//#include <iostream>
//
//#include "async_defs.h"
//
//
//namespace async {
//
//ThreadWorker::ThreadWorker() {
//    t = std::thread(&ThreadWorker::run, this);
//}
//
//ThreadWorker::~ThreadWorker() {
//    quit = true;
//    queue_cv.notify_all();
//    t.join();
//}
//
//void ThreadWorker::push_event(const Event& event) {
//    {
//        std::lock_guard<std::mutex> lock(queue_mutex);
//        event_queue.push(event);
//    }
//    queue_cv.notify_one();
//}
//
//void ThreadWorker::process_event(const Event& event) {
//
//    std::unique_lock<std::mutex> lock(executors_mutex);
//    auto it = g_executors.find(event.handle);
//    if (it != g_executors.end()) {
//        ExecutorPtr executor = it->second;
//        lock.unlock();
//        executor->receive_buffer(event.id, event.buffer);
//    }
//}
//
//void ThreadWorker::run() {
//    while (!quit) {
//        std::unique_lock<std::mutex> lock(queue_mutex);
//        queue_cv.wait(lock, [this]() {
//            return !event_queue.empty() || quit;
//        });
//        if (!quit) {
//            const Event& event = event_queue.front();
//            process_event(event);
//            event_queue.pop();
//        }
//    }
//}
//
//}
