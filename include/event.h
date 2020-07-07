#pragma once

#include <string>

#include "async.h"

namespace async {


struct Event {
    Event(std::size_t id, handle_t handle, const void* buffer, std::size_t size);
    
    std::size_t id {0};
    handle_t handle;
	std::string buffer;
};


}
