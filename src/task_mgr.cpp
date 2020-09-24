#include "task_mgr.h"

#include "job.h"
#include "job_manager.h"

namespace task_mgr {


void job_mgr_start() {
    JobManager::get_instance()->start();
}

void job_mgr_stop() {
    JobManager::get_instance()->stop();
}

void job_mgr_run_job_once(const JobConfigurator& job_config) {
    JobManager::get_instance()->run_job_once(job_config);
}

void job_mgr_run_job_delayed(const JobConfigurator& job_config, const std::chrono::milliseconds& delay) {
    JobManager::get_instance()->run_job_delayed(job_config, delay);
}

void job_mgr_run_job_scheduled(const JobConfigurator& job_config, const std::chrono::milliseconds& interval) {
    JobManager::get_instance()->run_job_scheduled(job_config, interval);
}

void job_mgr_cancel_job_by_tag(int tag) {
    JobManager::get_instance()->cancel_job_by_tag(tag);
}


const std::vector<TaskPtr>& JobConfigurator::get_tasks() const {
    return tasks;
}

const std::vector<std::pair<TaskPtr, TaskPtr>>& JobConfigurator::get_dependencies() const {
    return dependencies;
}

const std::function<void(IJob*)>& JobConfigurator::get_finish_callback() const {
    return finish_callback;
}

int JobConfigurator::get_tag() const {
    return tag;
}


}
