#pragma once

#include "executor.h"
#include "file_writer.h"
#include "console_printer.h"

class Application {
public:
    Application(size_t bulk_size);
    
	void run_main_loop();
private:
    std::shared_ptr<Executor> executor;
    std::unique_ptr<FileWriter> writer;
    std::unique_ptr<ConsolePrinter> printer;
};
