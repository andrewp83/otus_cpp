#pragma once

#include <memory>
#include <string>

// inner class ?

class Executor;

class ExecutorState {
public:
	ExecutorState(Executor* executor) : executor(executor) {}
    virtual ~ExecutorState() {}

    void set_executor(Executor* executor) { this->executor = executor; }
    
	virtual void parse_command(const std::string& name) = 0;

protected:
	Executor* executor;
};

class SimpleExecutorState : public ExecutorState {
public:
    SimpleExecutorState(Executor* executor) : ExecutorState(executor) {}
    
	virtual void parse_command(const std::string& name) override;
};

class BracedExecutorState : public ExecutorState {
public:
    BracedExecutorState(Executor* executor) : ExecutorState(executor) {}
    
	virtual void parse_command(const std::string& name) override;
    
private:
    size_t inner_braces {0};
};

using ExecutorStatePtr = std::shared_ptr<ExecutorState>;
