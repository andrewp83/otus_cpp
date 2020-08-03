#include "async.h"

#include <map>
#include <thread>
#include <iostream>

#include "console_printer.h"
#include "file_writer.h"
#include "lib_manager.h"
#include "command_publisher.hpp"
#include "executor.h"
#include "event.h"
#include "thread_worker.h"


namespace async {

LibManager lib;

handle_t connect(std::size_t block_size) {
    return lib.connect(block_size);
}


void receive(handle_t h, const void* buffer, std::size_t size) {
    lib.receive(h, buffer, size);
}


void disconnect(handle_t h) {
    lib.disconnect(h);
}


}
