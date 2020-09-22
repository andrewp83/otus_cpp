#pragma once

#include <chrono>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "thread_pool.h"
#include "job.h"

namespace task_mgr {


class JobManager {
public:
    static JobManager* get_instance();
    
    void start();
    void stop();
    
    void run_job_once(const Job::Configurator& job_config);
    
    void run_job_delayed(const Job::Configurator& job_config, const std::chrono::milliseconds& delay);
    
    void run_job_repeatedly(const Job::Configurator& job_config, const std::chrono::milliseconds& interval);

private:
    JobManager() {}
    JobManager(const JobManager&) = delete;
    JobManager(JobManager&&) = delete;
    void operator=(const JobManager&) = delete;
    void operator=(JobManager&&) = delete;
    
    ~JobManager();
    
    void update_jobs_schedule();
    
    void manage_jobs();
    
private:
    static JobManager* _instance;
    
    ThreadPool* thread_pool {nullptr};
    
    struct JobRunInfo {
        JobRunInfo(JobPtr job, std::time_t next_run_time, std::time_t interval = 0)
        : job(job)
        , next_run_time(next_run_time)
        , interval(interval) {}
        
        JobPtr job;
        std::chrono::milliseconds next_run_time;
        std::chrono::milliseconds interval;
        
        bool operator<(const JobRunInfo& other) const {
            return next_run_time < other.next_run_time;
        }
    };
    
    std::priority_queue<JobRunInfo> jobs;
    
    std::list<JobRunInfo> running_jobs;
    
    std::thread t;
    std::condition_variable cv;
    std::mutex cv_m;
    
    std::atomic<bool> quit {false};
};


}
