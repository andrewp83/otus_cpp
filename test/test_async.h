#include <gtest/gtest.h>

#include <queue>
#include <thread>
#include <chrono>

#include "command_observer.h"
#include "executor.h"
#include "async_defs.h"

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
//		observer->subscribe(executor);
        
        auto p = executor_map.emplace(4, std::make_shared<Executor>(4));
        observer->subscribe(p.first->second);
	}

	virtual void TearDown() {
		// ???
	}


	void set_executor_commands() {
        auto it = executor_map.find(4);
        ExecutorPtr exec = it->second;
		for (const auto& cmd : commands) {
            exec->parse_command(cmd);
		}
	}

	std::vector<std::string> commands;

	std::shared_ptr<Executor> executor;
	std::shared_ptr<TestObserver> observer;
    
    std::map<int, ExecutorPtr> executor_map;
};

//TEST_F(ExecutorTest, Simple) {
//	// Adjust ...
//	//
//
//	commands = {
//		"cmd1",
//		"cmd2",
//		"cmd3",
//		"cmd4"
//	};
//	set_executor_commands();
//
//	BulkResult res = observer->pop_last_result();
//    ASSERT_EQ(res.to_string(), "bulk: cmd1, cmd2, cmd3, cmd4");
//}
//
//TEST_F(ExecutorTest, Bulk) {
// 	commands = {
//		"cmd1",
//		"cmd2",
//		"cmd3",
//		"cmd4",
//		"{",
//		"cmd5",
//		"cmd6",
//		"cmd7",
//		"cmd8",
//		"cmd9",
//		"cmd10",
//		"}",
//		"}",
//	};
//	set_executor_commands();
//
//	BulkResult res = observer->pop_last_result();
//    ASSERT_EQ(res.to_string(), "bulk: cmd1, cmd2, cmd3, cmd4");
//	res = observer->pop_last_result();
//    ASSERT_EQ(res.to_string(), "bulk: cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
//	res = observer->pop_last_result();
//    ASSERT_EQ(res.to_string(), "");
//}
//
//TEST_F(ExecutorTest, NestedBulks) {
//   	commands = {
//		"cmd1",
//		"cmd2",
//		"cmd3",
//		"{",
//		"cmd4",
//		"cmd5",
//		"cmd6",
//		"{",
//		"cmd7",
//		"}",
//		"cmd8",
//		"{",
//		"cmd9",
//		"}",
//		"cmd10",
//		"}",
//	};
//	set_executor_commands();
//
//    std::string res = observer->pop_last_result().to_string();
//	ASSERT_EQ(res, "bulk: cmd1, cmd2, cmd3");
//    res = observer->pop_last_result().to_string();
//	ASSERT_EQ(res, "bulk: cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
//    res = observer->pop_last_result().to_string();
//	ASSERT_EQ(res, "");
//}

#include <chrono>

TEST_F(ExecutorTest, EmptyBulk) {
    commands = {
	};
	set_executor_commands();

	BulkResult res = observer->pop_last_result();
    ASSERT_EQ(res.to_string(), "");
    
    
    std::cout << "********\n";

    auto h = async::connect(5);
    std::string block1 = "cmd1\ncmd2\ncmd";
    async::receive(h, block1.c_str(), block1.size());
    std::string block2 = "3\n\n\n\ncmd4\n";
    async::receive(h, block2.c_str(), block2.size());
    std::string block3 = "cmd5\n";
    async::receive(h, block3.c_str(), block3.size());

    using namespace std::chrono_literals;

    std::this_thread::sleep_for(2s);

    std::cout << "********\n";
}


