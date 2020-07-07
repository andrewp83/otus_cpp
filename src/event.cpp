//
//  event.cpp
//  async
//
//  Created by a.polyakov on 02.07.2020.
//

#include "event.h"

namespace async {

Event::Event(std::size_t id, handle_t handle, const void* buffer, std::size_t size)
: id(id)
, handle(handle) {
    this->buffer = std::string(static_cast<const char*>(buffer), size);
}


}
