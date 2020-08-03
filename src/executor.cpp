#include "executor.h"

#include <algorithm>
#include <iostream>
#include "command_publisher.hpp"
#include "async_defs.h"

Executor::Executor(size_t bulk_size) 
: bulk_size(bulk_size) {
	simple_state = std::make_shared<SimpleExecutorState>(this);
	braced_state = std::make_shared<BracedExecutorState>(this);
	current_state = simple_state;
}

Executor::Executor(Executor&& other)
: commands(std::move(other.commands))
, raw_buffer(std::move(other.raw_buffer))
, current_state(std::move(other.current_state))
, simple_state(std::move(other.simple_state))
, braced_state(std::move(other.braced_state))
, bulk_size(std::move(other.bulk_size)) {
    
    simple_state->set_executor(this);
    braced_state->set_executor(this);
}

Executor& Executor::operator=(Executor&& other) {
    if (this == &other) {
        return *this;
    }

    commands = std::move(other.commands);
    raw_buffer = std::move(other.raw_buffer);
    bulk_size = std::move(other.bulk_size);
    current_state = std::move(other.current_state);
    simple_state = std::move(other.simple_state);
    braced_state = std::move(other.braced_state);
        
    simple_state->set_executor(this);
    braced_state->set_executor(this);
    
    return *this;
}

void Executor::parse_command(const std::string& name) {
    async::lib.notify_command_read(name);
	current_state->parse_command(name);
}

void Executor::receive_buffer(std::size_t event_id, const std::string& buffer) {
    std::lock_guard<std::recursive_mutex> lock(exec_mutex);
    received_buffers[event_id] = buffer;
    parse_buffers();
}

void Executor::parse_buffers() {
    if (received_buffers.empty()) {
        return;
    }
    
    std::size_t max_received_buffer_id = received_buffers.rbegin()->first;
    if (received_buffers.size() == (max_received_buffer_id - last_parsed_buffer_id)) {
        std::for_each(received_buffers.begin(), received_buffers.end(), [this](const std::pair<std::size_t, std::string>& _p){
            this->parse_buffer(_p.second);
        });
        received_buffers.clear();
        last_parsed_buffer_id = max_received_buffer_id;
    }
}

void Executor::parse_buffer(const std::string& buffer) {
    raw_buffer += buffer;
    std::size_t next_pos = 0;
    while ((next_pos = raw_buffer.find("\n")) != std::string::npos) {
        std::string next_cmd = raw_buffer.substr(0, next_pos);
        if (!next_cmd.empty()) {
            parse_command(next_cmd);
        }
        raw_buffer.erase(0, next_pos + 1);
    }
}

void Executor::add_command(const std::string& str) {
	std::unique_ptr<Command> cmd = CommandCreator::create(str);
	if (!cmd) {
		throw std::logic_error("unknown command");
	}
	commands.push(std::move(cmd));
}

BulkResult Executor::execute_bulk() {
    BulkResult bulk_res;
          
    std::lock_guard<std::recursive_mutex> exec_lock(exec_mutex);
    
    if (commands.empty()) return bulk_res;
    
	while (!commands.empty()) {
        std::unique_ptr<Command> cmd = std::move(commands.front());
        commands.pop();
		CommandResult cmd_res = cmd->execute();
        bulk_res.command_results.push_back(cmd_res);
	}
    
    async::lib.notify_bulk_executed(bulk_res);

    return bulk_res;
}

void Executor::set_simple_state() {
	current_state = simple_state;
}

void Executor::set_braced_state() {
	current_state = braced_state;
}
