#include "executor.h"
#include <iostream>
#include "command_observer.h"

Executor::Executor(size_t bulk_size) 
: bulk_size(bulk_size) {
	simple_state = std::make_shared<SimpleExecutorState>(this);
	braced_state = std::make_shared<BracedExecutorState>(this);
	current_state = simple_state;
}

void Executor::parse_command(const std::string& name) {
	current_state->parse_command(name);
}

void Executor::add_command(const std::string& str) {
	std::unique_ptr<Command> cmd = CommandCreator::create(str);
	if (!cmd) {
		throw std::logic_error("unknown command");
	}
	commands.push(std::move(cmd));
}

CommandResult Executor::execute_bulk() {
    CommandResult bulk_res;
    
    if (commands.empty()) return bulk_res;
    
	bulk_res = "bulk: ";
	while (!commands.empty()) {
        std::unique_ptr<Command> cmd = std::move(commands.front());
        commands.pop();
		CommandResult cmd_res = cmd->execute();
		bulk_res += cmd_res;
        if (!commands.empty()) bulk_res += ", ";
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
