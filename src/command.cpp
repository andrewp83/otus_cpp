#include "command.h"

CommandResult PrintCommand::execute() {
	return CommandResult(name);
}

std::unique_ptr<Command> CommandCreator::create(const std::string& name) {
	if (name == "{" || name == "}") {
		throw std::logic_error("wrong command: " + name);
	}
	return std::make_unique<PrintCommand>(name);
}
