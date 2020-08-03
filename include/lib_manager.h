#pragma once

#include <map>
#include <thread>
#include <vector>

#include "async.h"
#include "command_publisher.hpp"
#include "console_printer.h"
#include "executor.h"
#include "file_writer.h"
#include "thread_worker.h"

namespace async {

class LibManager {
public:
    LibManager();
    
    handle_t connect(std::size_t block_size);
    void receive(handle_t h, const void* buffer, std::size_t size);
    void disconnect(handle_t h);
    
    void push_event(const Event& event);
    void process_event(const Event& event);
    
    void notify_command_read(const std::string& name);
    void notify_bulk_executed(const BulkResult& result);
    
    std::condition_variable& get_queue_cv() { return queue_cv; }
    std::mutex& get_queue_mutex() { return queue_mutex; }
    const std::queue<Event>& get_event_queue() const { return event_queue; }
    void event_queue_pop();
    
private:
    const std::size_t THREADS_COUNT = 4;

    std::map<handle_t, ExecutorPtr> executors;
    std::mutex executors_mutex;
    std::size_t handlers_counter = 0;
    std::mutex workers_mutex;
    std::mutex publisher_mutex;

    std::queue<Event> event_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    
    std::vector<ThreadWorker> workers;

    std::map<std::size_t, std::size_t> event_ids; // handle => next_id

    std::shared_ptr<ConsolePrinter> printer;
    std::shared_ptr<FileWriter> file_writer;
    
    Publisher<CommandObserver> publisher;
};


}

