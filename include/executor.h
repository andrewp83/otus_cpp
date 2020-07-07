#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <map>

#include "common_types.h"
#include "command.h"
#include "executor_state.h"


class Executor {
public:
	Executor(size_t bulk_size);
    Executor(const Executor&) = delete;
    Executor(Executor&&);
    
    Executor& operator=(const Executor&) = delete;
    Executor& operator=(Executor&&);
    
    void receive_buffer(std::size_t event_id, const std::string& buffer);
    
    BulkResult execute_bulk();

    BulkResult get_last_result() const;

private:
    void parse_buffers();
    void parse_buffer(const std::string& buffer);
    
    void parse_command(const std::string& name);
	void add_command(const std::string& str);

	void set_simple_state();
	void set_braced_state();

private:
	std::queue<std::unique_ptr<Command>> commands;
    
    std::map<std::size_t, std::string> received_buffers;
    std::size_t last_parsed_buffer_id {0};
    
    std::string raw_buffer;

	ExecutorStatePtr current_state;
	ExecutorStatePtr simple_state;
	ExecutorStatePtr braced_state;

	size_t bulk_size {0};
    
    std::recursive_mutex exec_mutex;

	friend class SimpleExecutorState;
    friend class BracedExecutorState;
};

using ExecutorPtr = std::shared_ptr<Executor>;
