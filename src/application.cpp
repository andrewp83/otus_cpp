#include "application.h"

#include <iostream>

#include "executor.h"
#include "file_writer.h"

Application::Application(size_t bulk_size) {
    executor = std::make_shared<Executor>(bulk_size);
    
    writer = std::make_unique<FileWriter>();
    writer->subscribe(executor);
    
    printer = std::make_unique<ConsolePrinter>();
    printer->subscribe(executor);
}

void Application::run_main_loop() {
    std::string cmd_str;
    while (std::cin >> cmd_str) {
        executor->parse_command(cmd_str);
    }
    executor->execute_bulk();
}
