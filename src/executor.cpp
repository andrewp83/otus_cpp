#include "executor.h"
#include <iostream>
#include "command_observer.h"

Executor::Executor(size_t bulk_size) 
: bulk_size(bulk_size) {
	simple_state = std::make_shared<SimpleExecutorState>(this);
	braced_state = std::make_shared<BracedExecutorState>(this);
	current_state = simple_state;
}

bool Executor::parse_command(const std::string& name) {
    
    if (name == "exit") {
        execute_bulk();
        notify(&CommandObserver::command_process_stopped);
        return true;
    }
    
    notify(&CommandObserver::command_read, name);

	current_state->parse_command(name);
    
    return false;
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
	
	notify(&CommandObserver::bulk_executed, bulk_res);

    return bulk_res;
}

void Executor::set_simple_state() {
	current_state = simple_state;
}

void Executor::set_braced_state() {
	current_state = braced_state;
}
