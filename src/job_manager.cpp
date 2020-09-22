#include "job_manager.h"


namespace task_mgr {

JobManager* JobManager::_instance = nullptr;

JobManager* JobManager::get_instance() {
    if (!_instance) {
        _instance = new JobManager();
    }
    return _instance;
}

void JobManager::start() {
    if (!thread_pool) {
        quit = false;
        thread_pool = new ThreadPool();
        t = std::thread(&JobManager::manage_jobs, this);
    }
}

void JobManager::stop() {
    if (thread_pool) {
        thread_pool->stop();
        delete thread_pool;
        thread_pool = nullptr;
        //
        quit = true;
        cv.notify_one();
        t.join();
    }
}

void JobManager::run_job_once(const Job::Configurator& job_config) {
    JobPtr job = std::make_shared<Job>(job_config, thread_pool);
    {
        std::lock_guard<std::mutex> lock(cv_m);
        jobs.emplace(job, 0);
    }
    cv.notify_one();
    //update_jobs_schedule();
}

void JobManager::run_job_delayed(const Job::Configurator& job_config, const std::chrono::milliseconds& delay) {
    JobPtr job = std::make_shared<Job>(job_config, thread_pool);
    {
        std::lock_guard<std::mutex> lock(cv_m);
        auto start = std::chrono::system_clock::now();
        start += delay;
        jobs.emplace(job, start);
    }
    cv.notify_one();
}

void JobManager::run_job_repeatedly(const Job::Configurator& job_config, const std::chrono::milliseconds& interval) {
    JobPtr job = std::make_shared<Job>(job_config, thread_pool);
    {
        std::lock_guard<std::mutex> lock(cv_m);
        auto start = std::chrono::system_clock::now();
        start += interval;
        jobs.emplace(job, start);
    }
    cv.notify_one();
}

void JobManager::update_jobs_schedule() {
    
}

void JobManager::manage_jobs() {
    while (!quit) {
        std::unique_lock<std::mutex> lk(cv_m);
        
        if (!jobs.empty()) {
            const JobRunInfo& job_info = jobs.top();
            auto next_time = job_info.next_run_time;
            auto status = cv.wait_until(lk, next_time);
            if (status == std::cv_status::timeout) {
                // ВЫПОЛНИТЬ ЗАДАЧУ
                job_info.job->run();
                running_jobs.push_back(job_info);
                jobs.pop();
            }
        } else {
            // ЖДАТЬ ТОЛЬКО УСЛОВНУЮ ПЕРЕМЕННУЮ (ТО ЕСТЬ ПОКА В ОЧЕРЕДИ ПОЯВЯТСЯ ЗАДАЧИ)
            cv.wait(lk, [this]() { return !jobs.empty() || quit; });
        }
    }
}

JobManager::~JobManager() {
    stop();
}

}

