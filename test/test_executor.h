#include <gtest/gtest.h>

#include <queue>

#include "command_observer.h"
#include "executor.h"

class TestObserver : public CommandObserver {
public:
    void bulk_executed(const CommandResult& result) override {
    	last_results.push(result);
    }

    CommandResult pop_last_result() {
    	CommandResult res;
    	if (!last_results.empty()) {
    		res = last_results.front();
            last_results.pop();
    	}
    	return res;
    }

private:
	std::queue<CommandResult> last_results;

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

	CommandResult res = observer->pop_last_result();
	ASSERT_EQ(res, "bulk: cmd1, cmd2, cmd3, cmd4");
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

	CommandResult res = observer->pop_last_result();
	ASSERT_EQ(res, "bulk: cmd1, cmd2, cmd3, cmd4");
	res = observer->pop_last_result();
	ASSERT_EQ(res, "bulk: cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
	res = observer->pop_last_result();
	ASSERT_EQ(res, "");
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

	CommandResult res = observer->pop_last_result();
	ASSERT_EQ(res, "bulk: cmd1, cmd2, cmd3");
	res = observer->pop_last_result();
	ASSERT_EQ(res, "bulk: cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd10");
 	res = observer->pop_last_result();
	ASSERT_EQ(res, "");
}

TEST_F(ExecutorTest, EmptyBulk) {
    commands = {
	};
	set_executor_commands();

	CommandResult res = observer->pop_last_result();
	ASSERT_EQ(res, "");

}


// TEST(Matrix, Creation) {
//     // Arrange
//     Matrix<int, -1> matrix;
    
//     // Act
    
//     // Assert
//     ASSERT_EQ(matrix.size(), 0);
// }

// TEST(Matrix, ReturnDefault) {
//     Matrix<int, -1> matrix;
//     auto a = matrix[0][0];
//     ASSERT_EQ(a, -1);
//     ASSERT_EQ(matrix.size(), 0);
// }

// TEST(Matrix, Assign) {
//     Matrix<int, -1> matrix;
//     matrix[100][100] = 314;
//     ASSERT_EQ(matrix[100][100], 314);
// }

// TEST(Matrix, Iteration) {
    
//     Matrix<int, -1> matrix;
    
//     matrix[100][100] = 314;
//     matrix[23][44] = 52;
    
//     std::stringstream ss;
//     for(auto c : matrix) {
//         int x;
//         int y;
//         int v;
//         std::tie(x, y, v) = c;
//         ss << x << y << v;
//     }
    
//     // РЕЗУЛЬТАТ ИМЕННО В ТАКОМ ПОРЯДКЕ ПОТОМУ ЧТО КЛЮЧИ В std::map ОТСОРТИРОВАНЫ
//     ASSERT_EQ("234452100100314", ss.str());
    
// }

// TEST(Matrix, MultiAssign) {
//     Matrix<int, -1> matrix;
//     ((matrix[100][100] = 314) = 0) = 217;
//     ASSERT_EQ(matrix[100][100], 217);
// }
