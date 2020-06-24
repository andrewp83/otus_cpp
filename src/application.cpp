#include "application.h"

#include <iostream>

#include "executor.h"
#include "file_writer.h"


Application::Application(size_t bulk_size) {
    executor = std::make_shared<Executor>(bulk_size);
    
    thread_manager = std::make_unique<ThreadManager>();
    thread_manager->subscribe(executor);
}

void Application::run_main_loop() {
    std::string cmd_str;
    bool is_exit = false;
    while (!is_exit && std::cin >> cmd_str) {
        is_exit = executor->parse_command(cmd_str);
    }
    // И где-то тут обработать и показать статистику
}
