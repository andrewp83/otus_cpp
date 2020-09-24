#pragma once

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

class Task;

using TaskPtr = std::shared_ptr<Task>;

struct IJob {
    virtual TaskPtr get_task_by_tag(int tag) const = 0;
};

class Task {
public:
    template<class T, class... Args>
    static TaskPtr create(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    virtual ~Task() {}
    
    const TaskResult& get_result() const { return result; }
    
    int get_tag() const { return tag; }
    void set_tag(int tag) { this->tag = tag; }
        
    virtual void run(IJob* job) = 0;
    
    virtual void callback(IJob* /*job*/) {}
    
protected:
    Task() = default;
    
protected:
    TaskResult result;
    int tag {0};
};


struct IJobConfigurator {
    virtual void add_task(TaskPtr task) = 0;
    virtual void add_dependency(TaskPtr target, TaskPtr source) = 0;
    virtual void set_finish_callback(const std::function<void(IJob*)>& callback) = 0;
    virtual void set_tag(int tag) = 0;
};

std::unique_ptr<IJobConfigurator> create_job_configurator();


void job_mgr_start();
void job_mgr_stop();

void job_mgr_run_job_once(const IJobConfigurator* job_config);

void job_mgr_run_job_delayed(const IJobConfigurator* job_config, const std::chrono::milliseconds& delay);

void job_mgr_run_job_scheduled(const IJobConfigurator* job_config, const std::chrono::milliseconds& interval);

void job_mgr_cancel_job_by_tag(int tag);




/*
 
 class Task {
     friend class Job;
 public:
     template<class T, class... Args>
     static TaskPtr create(Args&&... args) {
         return std::make_shared<T>(std::forward<Args>(args)...);
     }
     
     virtual ~Task() {}
     
     IJob* get_job() { return job; }
     
     const TaskResult& get_result() const { return result; }
     
     int get_tag() const { return tag; }
     void set_tag(int tag) { this->tag = tag; }
     
     bool is_finished() const { return _is_finished; }
     
     virtual void run() = 0;
     
     virtual void callback() {}
     
 protected:
     Task() = default;
     
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

 
 
 */

}
