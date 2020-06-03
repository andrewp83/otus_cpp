#include "executor_state.h"
#include "executor.h"

void SimpleExecutorState::parse_command(const std::string& name) {
	if (name == "{") {
		executor->execute_bulk();
		executor->set_braced_state();
	} else if (name == "}") {
		// ИГНОР
	} else {
		executor->add_command(name);
		if (executor->commands.size() == executor->bulk_size) {
			executor->execute_bulk();
		}
	}
}


// BracedExecutorState

void BracedExecutorState::parse_command(const std::string& name) {
	if (name == "{") {
		inner_braces++;
	} else if (name == "}") {
		if (inner_braces == 0) {
			executor->execute_bulk();
			executor->set_simple_state();
		} else {
			inner_braces--;
		}
	} else {
		executor->add_command(name);
	}
}
