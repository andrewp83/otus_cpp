#include "lib_manager.h"

namespace async {

LibManager::LibManager()
: workers(THREADS_COUNT) {
    
    printer = std::make_shared<ConsolePrinter>();
    publisher.add(printer);

    file_writer = std::make_shared<FileWriter>();
    publisher.add(file_writer);
}

handle_t LibManager::connect(std::size_t block_size) {
    
    std::lock_guard<std::mutex> lock(executors_mutex);
    
    ExecutorPtr executor = std::make_shared<Executor>(block_size);
    executors.emplace(++handlers_counter, executor);
    
    event_ids[handlers_counter] = 0;
    
    return handlers_counter;
}


void LibManager::receive(handle_t h, const void* buffer, std::size_t size) {
    
    std::lock_guard<std::mutex> lock(workers_mutex);
   
    auto it = event_ids.find(h);
    std::size_t event_id = ++it->second;
    push_event(Event(event_id, h, buffer, size));
}


void LibManager::disconnect(handle_t h) {
    std::lock_guard<std::mutex> lock(executors_mutex);
    auto it = executors.find(h);
    if (it != executors.end()) {
        ExecutorPtr executor = it->second;
        executor->execute_bulk();
        executors.erase(it);
    }
}

void LibManager::add_observer(const std::shared_ptr<CommandObserver>& obs) {
    publisher.add(obs);
}

void LibManager::set_testing_mode() {
    printer.reset();
    file_writer.reset();
}

void LibManager::push_event(const Event& event) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        event_queue.push(event);
    }
    queue_cv.notify_one();
}

void LibManager::process_event(const Event &event) {
    std::unique_lock<std::mutex> lock(executors_mutex);
    auto it = executors.find(event.handle);
    if (it != executors.end()) {
       ExecutorPtr executor = it->second;
       lock.unlock();
       executor->receive_buffer(event.id, event.buffer);
    }
}

void LibManager::event_queue_pop() {
    event_queue.pop();
}

void LibManager::notify_command_read(const std::string &name) {
    std::unique_lock<std::mutex> lock(publisher_mutex);
    publisher.notify(&CommandObserver::command_read, name);
    lock.unlock();
}

void LibManager::notify_bulk_executed(const BulkResult &result) {
    std::unique_lock<std::mutex> lock(publisher_mutex);
    publisher.notify(&CommandObserver::bulk_executed, result);
}


}
