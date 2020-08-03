#pragma once

#include <atomic>
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
    ~ThreadWorker();
    
	void run();
    
private:
    void process_event(const Event& event);

private:
    std::thread t;
    
    std::atomic<bool> quit {false};
};

}
