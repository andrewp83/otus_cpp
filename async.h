#pragma once

#include <cstddef>

namespace async {


using handle_t = std::size_t;

handle_t connect(std::size_t block_size);

void receive(handle_t h, const void* buffer, std::size_t size);

void disconnect(handle_t h);


}
