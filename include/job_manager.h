#pragma once

namespace task_mgr {


class JobManager {
public:
    static JobManager* get_instance();

private:
    JobManager() {}
    JobManager(const JobManager&) = delete;
    JobManager(JobManager&&) = delete;
    void operator=(const JobManager&) = delete;
    void operator=(JobManager&&) = delete;
    
private:
    static JobManager* _instance;
};


}
