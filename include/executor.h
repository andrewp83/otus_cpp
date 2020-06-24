#pragma once

#include <memory>
#include <queue>

#include "common_types.h"
#include "command.h"
#include "command_publisher.hpp"
#include "executor_state.h"

class Executor : public CommandPublisher {
public:
	Executor(size_t bulk_size);

	bool parse_command(const std::string& name);
    
    BulkResult execute_bulk();

    BulkResult get_last_result() const;

private:
	void add_command(const std::string& str);

	void set_simple_state();
	void set_braced_state();

private:
	std::queue<std::unique_ptr<Command>> commands;

	ExecutorStatePtr current_state;
	ExecutorStatePtr simple_state;
	ExecutorStatePtr braced_state;

	const size_t bulk_size {0};

	friend class SimpleExecutorState;
    friend class BracedExecutorState;
};

using ExecutorPtr = std::shared_ptr<Executor>;
