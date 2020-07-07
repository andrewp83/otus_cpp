#include "async.h"

#include <map>
#include <thread>
#include <iostream>

#include "console_printer.h"
#include "command_publisher.hpp"
#include "executor.h"
#include "event.h"
#include "thread_worker.h"


namespace async {

std::shared_ptr<ConsolePrinter> create_printer() {
    std::shared_ptr<ConsolePrinter> printer = std::make_shared<ConsolePrinter>();
    Publisher<CommandObserver>::add(printer);
    return printer;
}

const std::size_t THREADS_COUNT = 4;

std::map<handle_t, ExecutorPtr> g_executors;
std::mutex g_executors_mutex;
std::size_t g_handlers_counter = 0;
std::size_t g_current_worker = 0;
std::mutex g_workers_mutex;
std::mutex g_publisher_mutex;

std::vector<ThreadWorker> g_workers(THREADS_COUNT);

std::map<std::size_t, std::size_t> g_event_ids; // handle => next_id

std::shared_ptr<ConsolePrinter> g_printer = create_printer();

handle_t connect(std::size_t block_size) {
    std::lock_guard<std::mutex> lock(g_executors_mutex);
    
    ExecutorPtr executor = std::make_shared<Executor>(block_size);
    g_executors.emplace(++g_handlers_counter, executor);
    
    g_event_ids[g_handlers_counter] = 0;
    
    return g_handlers_counter;
}


void receive(handle_t h, const void* buffer, std::size_t size) {
    std::lock_guard<std::mutex> lock(g_workers_mutex);
    if (++g_current_worker >= g_workers.size()) {
        g_current_worker = 0;
    }
    auto it = g_event_ids.find(h);
    std::size_t event_id = ++it->second;
    g_workers[g_current_worker].push_event(Event(event_id, h, buffer, size));
}


void disconnect(handle_t h) {
    std::lock_guard<std::mutex> lock(g_executors_mutex);
    auto it = g_executors.find(h);
    if (it != g_executors.end()) {
        ExecutorPtr executor = it->second;
        executor->execute_bulk();
        g_executors.erase(it);
    }
}



}
