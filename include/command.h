#pragma once

#include <memory>
#include <string>

#include "common_types.h"


class Command {
public:
	Command(const std::string& name) : name(name) {}
    virtual ~Command() {}

	virtual CommandResult execute() = 0;

protected:
	std::string name;
};


class PrintCommand : public Command {
public:
    PrintCommand(const std::string& name) : Command(name) {}
    
	virtual CommandResult execute() override;
};


class CommandCreator {
public:
	static std::unique_ptr<Command> create(const std::string& name);
};

