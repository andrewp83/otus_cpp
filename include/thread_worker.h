#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>

#include "async.h"
#include "event.h"

namespace async {

class ThreadWorker {
public:
    ThreadWorker();

    void push_event(const Event& event);
    
	void run();
    
private:
    void process_event(const Event& event);

private:
    std::queue<Event> event_queue;
    std::condition_variable queue_cv;
    std::mutex queue_mutex;
    
    std::thread t;
    
    bool quit {false};
};

}
