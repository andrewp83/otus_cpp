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
    Executor(const Executor&) = delete;
    Executor(Executor&&);
    
    Executor& operator=(const Executor&) = delete;
    Executor& operator=(Executor&&);
    
    void parse_buffer(const std::string& buffer);
    
	void parse_command(const std::string& name);
    
    BulkResult execute_bulk();

    BulkResult get_last_result() const;

private:
	void add_command(const std::string& str);

	void set_simple_state();
	void set_braced_state();

private:
	std::queue<std::unique_ptr<Command>> commands;
    
    std::string raw_buffer;

	ExecutorStatePtr current_state;
	ExecutorStatePtr simple_state;
	ExecutorStatePtr braced_state;

	size_t bulk_size {0};

	friend class SimpleExecutorState;
    friend class BracedExecutorState;
};

using ExecutorPtr = std::shared_ptr<Executor>;
