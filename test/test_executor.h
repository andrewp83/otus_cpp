#include <gtest/gtest.h>

#include <queue>
#include <thread>
#include <chrono>

#include "command_observer.h"
#include "executor.h"
#include "thread_manager.h"
//#include "stats_manager.h"

class TestObserver : public CommandObserver {
public:
    void bulk_executed(const BulkResult& result) override {
    	last_results.push(result);
    }

    BulkResult pop_last_result() {
    	BulkResult res;
    	if (!last_results.empty()) {
    		res = last_results.front();
            last_results.pop();
    	}
    	return res;
    }

private:
	std::queue<BulkResult> last_results;

};

class ExecutorTest : public ::testing::Test {
protected:
	virtual ~ExecutorTest() {
		std::cout << "~ExecutorTest()" << std::endl;
	}

	virtual void SetUp() {
		executor = std::make_shared<Executor>(4);
		observer = std::make_shared<TestObserver>();
		observer->subscribe(executor);
	}

	virtual void TearDown() {
		// ???
	}


	void set_executor_commands() {
		for (const auto& cmd : commands) {
			executor->parse_command(cmd);
		}
	}

	std::vector<std::string> commands;

	std::shared_ptr<Executor> executor;
	std::shared_ptr<TestObserver> observer;
};

TEST_F(ExecutorTest, Simple) {
	// Adjust ...
	//

	commands = {
		"cmd1", 
		"cmd2", 
		"cmd3", 
		"cmd4"
	};
	set_executor_commands();

	BulkResult res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "bulk: cmd1, cmd2, cmd3, cmd4");
}

TEST_F(ExecutorTest, Bulk) {
 	commands = {
		"cmd1", 
		"cmd2", 
		"cmd3", 
		"cmd4",
		"{",
		"cmd5",
		"cmd6",
		"cmd7",
		"cmd8",
		"cmd9",
		"cmd10",
		"}",
		"}",
	};
	set_executor_commands();

	BulkResult res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "bulk: cmd1, cmd2, cmd3, cmd4");
	res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "bulk: cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
	res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "");
}

TEST_F(ExecutorTest, NestedBulks) {
   	commands = {
		"cmd1", 
		"cmd2", 
		"cmd3", 
		"{",
		"cmd4",
		"cmd5",
		"cmd6",
		"{",
		"cmd7",
		"}",
		"cmd8",
		"{",
		"cmd9",
		"}",
		"cmd10",
		"}",
	};
	set_executor_commands();

    std::string res = observer->pop_last_result().to_string();
	ASSERT_EQ(res, "bulk: cmd1, cmd2, cmd3");
    res = observer->pop_last_result().to_string();
	ASSERT_EQ(res, "bulk: cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
    res = observer->pop_last_result().to_string();
	ASSERT_EQ(res, "");
}

TEST_F(ExecutorTest, EmptyBulk) {
    commands = {
	};
	set_executor_commands();

	BulkResult res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "");

}


// ТЕСТ МНОГОПОТОЧНОСТИ

class MultiThreadsTest : public ::testing::Test {
protected:
    virtual ~MultiThreadsTest() {
        
    }

    virtual void SetUp() {
        executor = std::make_shared<Executor>(4);
        
        threads_mgr = std::make_shared<ThreadManager>(ss_output);
        threads_mgr->subscribe(executor);
    }

    virtual void TearDown() {
        // ???
    }


    void set_executor_commands() {
        for (const auto& cmd : commands) {
            executor->parse_command(cmd);
        }
    }

    std::vector<std::string> commands;
    
    std::stringstream ss_output;

    std::shared_ptr<Executor> executor;
    //std::shared_ptr<TestObserver> observer;
    std::shared_ptr<ThreadManager> threads_mgr;
};

TEST_F(MultiThreadsTest, Base) {
    commands = {
        "{",
        "cmd1",
        "cmd2",
        "{",
        "cmd3",
        "cmd4",
        "}",
        "cmd5",
        "cmd6",
        "}",
        "exit",
    };
    
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
    set_executor_commands();
    
    std::stringstream ss_expected;
    ss_expected << "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6" << std::endl;
    ss_expected << "log thread: 1 bulks, 6 commands" << std::endl;
    ss_expected << "file1 thread_1: 1 bulks, 6 commands" << std::endl;
    ss_expected << "file1 thread_2: 0 bulks, 0 commands" << std::endl;

    threads_mgr->print_stats();

    ASSERT_EQ(ss_output.str(), ss_expected.str());
    
}
