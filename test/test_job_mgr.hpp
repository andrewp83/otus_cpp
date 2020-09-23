#pragma once

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "job_manager.h"
#include "job.h"
#include "task.h"

using namespace std::chrono_literals;
using namespace task_mgr;

// ПРОВЕРИТЬ ДОБАВЛЕНИЕ ОДИНОЧНОЙ РАБОТЫ

class TaskBase : public Task {
public:
    void run() override {
        std::this_thread::sleep_for(0.1s);
        res = 42;
        result = TaskResult(&res, sizeof(res));
        
        //std::cout << "TaskBase::run()" << std::endl;
    }
private:
    int res {0};
};

TEST(test_job_mgr, base_job) {
    JobManager::get_instance()->start();

    std::atomic<int> res = 0;

    Job::Configurator config;
    TaskPtr task = std::make_shared<TaskBase>();
    task->set_tag(42);
    config.add_task(task);
    config.set_finish_callback([&](IJob* job){
        TaskPtr task = job->get_task_by_tag(42);
        res = *((int*)(task->get_result().get_data()));
    });
    JobManager::get_instance()->run_job_once(config);

    while (!res) {
        std::this_thread::sleep_for(0.1s);
    }

    JobManager::get_instance()->stop();

    ASSERT_EQ(res, 42) << " base job failed";
}


// ЗАПУСК JOB-а C ЗАДЕРЖКОЙ
class JobManagerTest : public ::testing::Test {
protected:
    
    virtual void SetUp() {
        JobManager::get_instance()->start();
        
        res = total_res = 0;
        
        TaskPtr task = std::make_shared<TaskBase>();
        task->set_tag(42);
        job_simple_sum_config.add_task(task);
        job_simple_sum_config.set_finish_callback([&](IJob* job){
            TaskPtr task = job->get_task_by_tag(42);
            res = *((int*)(task->get_result().get_data()));
        });
        
        
        job_schedule_config.add_task(task);
        job_schedule_config.set_tag(33);
        job_schedule_config.set_finish_callback([&](IJob* job){
            TaskPtr task = job->get_task_by_tag(42);
            res = *((int*)(task->get_result().get_data()));
            total_res += res;
        });
        
    }

    virtual void TearDown() {
        JobManager::get_instance()->stop();
    }
    
    std::atomic<int> res;
    
    Job::Configurator job_simple_sum_config;
    Job::Configurator job_schedule_config;
    
    std::atomic<int> total_res;
};

TEST_F(JobManagerTest, DelayedRun) {
    
    JobManager::get_instance()->run_job_delayed(job_simple_sum_config, std::chrono::milliseconds(2000));

    while (!res) {
        std::this_thread::sleep_for(0.1s);
    }

    ASSERT_EQ(res, 42) << " base fix job failed";
}

TEST_F(JobManagerTest, SheduledRun) {
    
    JobManager::get_instance()->run_job_scheduled(job_schedule_config, std::chrono::milliseconds(1000));
    
    std::this_thread::sleep_for(6s);
    
    JobManager::get_instance()->cancel_job_by_tag(33);

    ASSERT_EQ(total_res, 42 * 5);
}

