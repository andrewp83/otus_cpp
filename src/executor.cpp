#include "executor.h"
#include <iostream>
#include "command_publisher.hpp"

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

    Publisher<CommandObserver>::notify(&CommandObserver::command_read, name);

	current_state->parse_command(name);
}

void Executor::parse_buffer(const std::string& buffer) {
    
    raw_buffer = buffer;
    
    std::size_t next_pos = 0;
    while ((next_pos = raw_buffer.find("\n")) != std::string::npos) {
        std::string next_cmd = raw_buffer.substr(0, next_pos);
        parse_command(next_cmd);
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
    
    if (commands.empty()) return bulk_res;
    
	while (!commands.empty()) {
        std::unique_ptr<Command> cmd = std::move(commands.front());
        commands.pop();
		CommandResult cmd_res = cmd->execute();
        bulk_res.command_results.push_back(cmd_res);
	}
	
	Publisher<CommandObserver>::notify(&CommandObserver::bulk_executed, bulk_res);

    return bulk_res;
}

void Executor::set_simple_state() {
	current_state = simple_state;
}

void Executor::set_braced_state() {
	current_state = braced_state;
}
