#pragma once

#include <memory>

#include "executor.h"
#include "file_writer.h"
#include "console_printer.h"
#include "stats_manager.h"
#include "thread_manager.h"


class Application {
public:
    Application(size_t bulk_size);
    
	void run_main_loop();
    
private:
    std::shared_ptr<Executor> executor;
    std::unique_ptr<ThreadManager> thread_manager;
    std::unique_ptr<StatsManager> stats_manager;
};
