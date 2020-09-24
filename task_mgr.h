#pragma once

#include <memory>
#include <functional>
#include <chrono>
#include <vector>


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

class JobConfigurator {
public:
    void add_task(TaskPtr task);
    void add_dependency(TaskPtr target, TaskPtr source);
    void set_finish_callback(const std::function<void(IJob*)>& callback);
    void set_tag(int tag);
    
    const std::vector<TaskPtr>& get_tasks() const;
    const std::vector<std::pair<TaskPtr, TaskPtr>>& get_dependencies() const;
    const std::function<void(IJob*)>& get_finish_callback() const;
    int get_tag() const;
    
private:
    std::vector<TaskPtr> tasks;
    std::vector<std::pair<TaskPtr, TaskPtr>> dependencies;
    std::function<void(IJob*)> finish_callback;
    int tag {0};
};


void job_mgr_start();
void job_mgr_stop();

void job_mgr_run_job_once(const JobConfigurator& job_config);

void job_mgr_run_job_delayed(const JobConfigurator& job_config, const std::chrono::milliseconds& delay);

void job_mgr_run_job_scheduled(const JobConfigurator& job_config, const std::chrono::milliseconds& interval);

void job_mgr_cancel_job_by_tag(int tag);

} // namespace task_mgr
