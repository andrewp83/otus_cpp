#pragma once

#include <atomic>
#include <memory>

namespace task_mgr {

class TaskResult {
public:
    TaskResult() {}
    TaskResult(void* data, std::size_t size) : data(data), size(size) {}
    
    const void* get_data() const { return data; }
    
    std::size_t get_size() { return size; }
    
protected:
    void* data {nullptr};
    std::size_t size {0};
};

class Job;

class Task {
    friend class Job;
public:
    virtual ~Task() {}
    
    Job* get_job() { return job; }
    
    const TaskResult& get_result() const { return result; }
    
    int get_tag() const { return tag; }
    void set_tag(int tag) { this->tag = tag; }
    
    bool is_finished() const { return _is_finished; }
    
    virtual void run() = 0;
    
    virtual void callback() {}
    
protected:
    friend class Job;
    
    void set_job(Job* job) { this->job = job; }
    
    void finish() {
        _is_finished = true;
        callback();
    }
    
    void init() {
        _is_finished = false;
    }
    
protected:
    Job* job {nullptr};
    
    TaskResult result;
    
    int tag {0};
    
    std::atomic<bool> _is_finished {0};
};

using TaskPtr = std::shared_ptr<Task>;


}
