#include "async.h"

#include <map>
#include <thread>

#include "console_printer.h"
#include "executor.h"
#include "event.h"
#include "thread_worker.h"


namespace async {

std::map<handle_t, ExecutorPtr> g_executors;
std::mutex g_executors_mutex;
std::size_t g_handlers_counter = 0;

ThreadWorker g_worker;


handle_t connect(std::size_t block_size) {
    std::lock_guard<std::mutex> lock(g_executors_mutex);
    
    ExecutorPtr executor = std::make_shared<Executor>(block_size);
    g_executors.emplace(++g_handlers_counter, executor);
    
    return g_handlers_counter;
}


void receive(handle_t h, const void* buffer, std::size_t size) {
    g_worker.push_event(Event(h, buffer, size));
}


void disconnect(handle_t h) {
    std::lock_guard<std::mutex> lock(g_executors_mutex);
    auto it = g_executors.find(h);
    if (it != g_executors.end()) {
        it->second->execute_bulk();
        g_executors.erase(it);
    }
}


}
