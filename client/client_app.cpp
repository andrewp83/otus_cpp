#include <iostream>
#include <chrono>
#include <thread>

#include "client_app.h"

using namespace std::chrono_literals;

int main(int argc, char **argv) {
    
    task_mgr::job_mgr_start();
    
    task_mgr::JobConfigurator config;
    task_mgr::TaskPtr task = task_mgr::Task::create<TaskPrint>();
    task->set_tag(42);
    config.add_task(task);
    config.set_finish_callback([&](task_mgr::IJob* job){
        std::cout << "finished" << std::endl;
    });
    
    task_mgr::job_mgr_run_job_once(config);

    std::this_thread::sleep_for(0.1s);

    task_mgr::job_mgr_stop();

    
    return 0;
}
