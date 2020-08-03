#include "thread_worker.h"

#include <iostream>

#include "async_defs.h"


namespace async {

ThreadWorker::ThreadWorker() {
    t = std::thread(&ThreadWorker::run, this);
}

ThreadWorker::~ThreadWorker() {
    quit = true;
    lib.get_queue_cv().notify_all();
    t.join();
}

void ThreadWorker::process_event(const Event& event) {
    lib.process_event(event);
}

void ThreadWorker::run() {
    while (!quit) {
        std::unique_lock<std::mutex> lock(lib.get_queue_mutex());
        lib.get_queue_cv().wait(lock, [this]() {
            return !lib.get_event_queue().empty() || quit;
        });
        if (!quit) {
            const Event& event = lib.get_event_queue().front();
            process_event(event);
            lib.event_queue_pop();
        }
    }
}

}
