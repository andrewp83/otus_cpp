#pragma once

#include "task_mgr.h"

class TaskPrint : public task_mgr::Task {
public:
    void run(task_mgr::IJob*) override {
       std::cout << "TaskBase::run()" << std::endl;
    }
};
