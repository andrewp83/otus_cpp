#pragma once

#include <stdexcept>
#include <string>

namespace task_mgr {


struct bad_task : public std::invalid_argument {
    bad_task(const std::string& what_arg) : std::invalid_argument(what_arg) {}
};

struct dependency_cycle : public bad_task {
    dependency_cycle() : bad_task("Task dependencies have a cycle.") {}
};

struct duplicate_task : public bad_task {
    duplicate_task() : bad_task("Task can be added only once.") {}
};

}

