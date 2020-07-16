#pragma once

#include <string>

namespace async {

using handle_t = std::size_t;

struct Event {
    Event(std::size_t id, handle_t handle, const void* buffer, std::size_t size);
    
    std::size_t id {0};
    handle_t handle;
	std::string buffer;
};


}
