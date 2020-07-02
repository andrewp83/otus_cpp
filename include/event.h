#pragma once

#include <string>

#include "async.h"

namespace async {


struct Event {
    Event(handle_t handle, const void* buffer, std::size_t size);
    
    handle_t handle;
	std::string buffer;
};


}
