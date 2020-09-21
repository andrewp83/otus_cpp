#include "job_manager.h"


namespace task_mgr {

JobManager* JobManager::_instance = nullptr;

JobManager* JobManager::get_instance() {
    if (!_instance) {
        _instance = new JobManager();
    }
    return _instance;
}


}

