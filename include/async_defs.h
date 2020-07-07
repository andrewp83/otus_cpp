//
//  async_defs.h
//  async
//
//  Created by a.polyakov on 02.07.2020.
//

#pragma once

#include <map>

#include "executor.h"
#include "thread_worker.h"

namespace async {

extern std::map<handle_t, ExecutorPtr> g_executors;
extern std::mutex g_executors_mutex;
extern std::size_t g_handlers_counter;
extern std::mutex g_publisher_mutex;

extern ThreadWorker g_worker;

}
