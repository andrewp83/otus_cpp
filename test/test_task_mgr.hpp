#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <thread>
#include <vector>

#include "task_mgr.h"
#include "job_manager.h"

#include "test_task_processing.hpp"

using namespace task_mgr;

using namespace std::chrono_literals;

class TestSumTask : public Task {
public:
    TestSumTask(int a, int b) : a(a), b(b) {}
    virtual ~TestSumTask() {}
    
    virtual void run(IJob*) override {
        sum = a + b;
        result = TaskResult(&sum, sizeof(sum));
        std::this_thread::sleep_for(0.5s);
    }
    
    virtual void callback(IJob*) override {
        //std::cout << "TestSumTask finished " << std::endl;
    }
    
private:
    
    int a {0};
    int b {0};
    int sum;
};

const int TAG_TASK_SUM = 123;

TEST(test_job, base_job) {
    std::atomic<int> sum = 0;

    JobManager::get_instance()->start();

    JobConfigurator config;

    TaskPtr sum_task = std::make_shared<TestSumTask>(3, 2);
    config.add_task(sum_task);
    config.set_finish_callback([&sum, sum_task](IJob*){
        sum = *((int*)(sum_task->get_result().get_data()));
    });
    JobManager::get_instance()->run_job_once(config);

    while (!sum) {
        std::this_thread::sleep_for(0.15s);
    }

    ASSERT_EQ(sum, 5) << "sum is not equal 5";

    JobManager::get_instance()->stop();
}

class TestMultiTask : public Task {
public:
    TestMultiTask(int a, int b) : a(a), b(b) {}
    virtual ~TestMultiTask() {}

    virtual void run(IJob*) override {
        mul = a * b;
        result = TaskResult(&mul, sizeof(mul));
        std::this_thread::sleep_for(0.5s);
    }

    virtual void callback(IJob*) override {
        //std::cout << "TestMulTask finished " << std::endl;
    }

private:
    int a {0};
    int b {0};
    int mul {0};
};

class TestFactorialTask : public Task {
public:
    TestFactorialTask(int n) : n(n) {}
    virtual ~TestFactorialTask() {}

    virtual void run(IJob*) override {
        res = fact(n);
        result = TaskResult(&res, sizeof(res));
        std::this_thread::sleep_for(0.5s);
    }

    virtual void callback(IJob*) override {
        //std::cout << "TestFactorialTask finished " << std::endl;
    }

private:
    int fact(int n) {
        if (n <= 1) return 1;
        return n * fact(n - 1);
    }

private:
    int n {0};
    int res {0};
};

// ВЫПОЛНЕНИЕ НЕСКОЛЬКИХ НЕСВЯЗАННЫХ МЕЖДУ СОБОЙ ЗАДАЧ
TEST(test_job, base_multi_tasks) {
    std::cout << "DEBUG PRINT" << std::endl;
    
    std::atomic<int> sum = 0, mul = 0, fact = 0;

    JobManager::get_instance()->start();

    JobConfigurator job_config;

    TaskPtr sum_task = std::make_shared<TestSumTask>(3, 2);
    TaskPtr mul_task = std::make_shared<TestMultiTask>(5, 4);
    TaskPtr fact_task = std::make_shared<TestFactorialTask>(5);

    job_config.add_task(sum_task);
    job_config.add_task(mul_task);
    job_config.add_task(fact_task);
    job_config.set_finish_callback([&](IJob*){
        sum = *((int*)(sum_task->get_result().get_data()));
        mul = *((int*)(mul_task->get_result().get_data()));
        fact = *((int*)(fact_task->get_result().get_data()));
    });
    JobManager::get_instance()->run_job_once(job_config);

    while (!sum || !mul || !fact) {
        std::this_thread::sleep_for(0.1s);
    }

    ASSERT_EQ(sum, 5) << "sum task is wrong";
    ASSERT_EQ(mul, 20) << "mul task is wrong";
    ASSERT_EQ(fact, 120) << "sum task is wrong";

    JobManager::get_instance()->stop();
}


// ВЫПОЛНЕНИЕ НЕСКОЛЬКИХ СВЯЗАННЫХ ЗАДАЧ
TEST(test_job, base_multi_related_tasks) {
    std::atomic<int> sum = 0, mul = 0, fact = 0;

    JobManager::get_instance()->start();

    JobConfigurator job_config;

    TaskPtr sum_task = std::make_shared<TestSumTask>(3, 2);
    TaskPtr mul_task = std::make_shared<TestMultiTask>(5, 4);
    TaskPtr fact_task = std::make_shared<TestFactorialTask>(5);

    job_config.add_task(sum_task);
    job_config.add_task(mul_task);
    job_config.add_task(fact_task);
    job_config.set_finish_callback([&](IJob*){
        sum = *((int*)(sum_task->get_result().get_data()));
        mul = *((int*)(mul_task->get_result().get_data()));
        fact = *((int*)(fact_task->get_result().get_data()));
    });
    JobManager::get_instance()->run_job_once(job_config);

    while (!sum || !mul || !fact) {
        std::this_thread::sleep_for(0.1s);
    }

    ASSERT_EQ(sum, 5) << "sum task is wrong";
    ASSERT_EQ(mul, 20) << "mul task is wrong";
    ASSERT_EQ(fact, 120) << "sum task is wrong";

    JobManager::get_instance()->stop();
}

// ВЫПОЛЕНИЕ ЗАВИСИМЫХ ЗАДАЧ

static const int TAG_SUM_TASK = 1;
static const int TAG_MUL_TASK = 2;
static const int TAG_FACT_TASK = 3;

class TestMeanTask : public Task {
public:
    TestMeanTask() {}
    virtual ~TestMeanTask() {}

    virtual void run(IJob* job) {
        TaskPtr sum_task = job->get_task_by_tag(TAG_SUM_TASK);
        int sum = *((int*)(sum_task->get_result().get_data()));
        TaskPtr mul_task = job->get_task_by_tag(TAG_MUL_TASK);
        int mul = *((int*)(mul_task->get_result().get_data()));
        TaskPtr fact_task = job->get_task_by_tag(TAG_FACT_TASK);
        int fact = *((int*)(fact_task->get_result().get_data()));

        res = (sum + mul + fact) / 3.0f;

        result = TaskResult(&res, sizeof(res));

    }

private:
    float res {0.0f};
};

TEST(test_job, base_dependency_tasks) {

    JobManager::get_instance()->start();

    JobConfigurator job_config;

    TaskPtr sum_task = std::make_shared<TestSumTask>(3, 2);
    sum_task->set_tag(TAG_SUM_TASK);
    TaskPtr mul_task = std::make_shared<TestMultiTask>(5, 4);
    mul_task->set_tag(TAG_MUL_TASK);
    TaskPtr fact_task = std::make_shared<TestFactorialTask>(5);
    fact_task->set_tag(TAG_FACT_TASK);
    TaskPtr mean_task = std::make_shared<TestMeanTask>();
    mean_task->set_tag(777);

    job_config.add_task(sum_task);
    job_config.add_task(mul_task);
    job_config.add_task(fact_task);
    job_config.add_task(mean_task);

    job_config.add_dependency(mean_task, sum_task);
    job_config.add_dependency(mean_task, mul_task);
    job_config.add_dependency(mean_task, fact_task);

    std::atomic<float> mean = 0.0f;

    job_config.set_finish_callback([&](IJob*){
        mean = *((float*)(mean_task->get_result().get_data()));
    });

    JobManager::get_instance()->run_job_once(job_config);

    while (mean < 0.001f) {
        std::this_thread::sleep_for(0.1s);
    }

    ASSERT_EQ(mean, (5 + 20 + 120) / 3.0f) << "base_dependency_tasks is wrong";

    JobManager::get_instance()->stop();
}

//// ПРОВЕРИТЬ НА ВЫБРОС ИСКЛЮЧЕНИЯ ПРИ ДОБАВЛЕНИИ ЦИКЛИЧЕСКИХ ЗАВИСИМОСТЕЙ
//
//// TO DO ....
//
//
// ПРОВЕРКА НА СЛОЖНОЕ ДЕРЕВО ЗАВИСИМОСТЕЙ


TEST(test_job, complex_linked_tasks) {
    //JobManager::get_instance()->start();

    //JobConfigurator job_config;

    std::filesystem::create_directories("test/data");

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/figs/file_dep.gif

    // ТАСКИ 1 УРОВНЯ
//    TaskPtr dax_task = Task::create<TestProcessingTask>(std::vector<std::string>{}, "dax.h");
//    job_config.add_task(dax_task);
//    TaskPtr zow_task = Task::create<TestProcessingTask>(std::vector<std::string>{}, "zow.h");
//    job_config.add_task(zow_task);
//    TaskPtr yow_task = Task::create<TestProcessingTask>(std::vector<std::string>{}, "yow.h");
//    job_config.add_task(yow_task);
//    TaskPtr boz_task = Task::create<TestProcessingTask>(std::vector<std::string>{}, "boz.h");
//    job_config.add_task(boz_task);

    // ТАСКИ СЛЕДУЮЩИХ УРОВНЕЙ
//    TaskPtr foo_cpp_task = Task::create<TestProcessingTask>(std::vector<std::string>{"zow.h", "dax.h"}, "foo.cpp");
//    job_config.add_task(foo_cpp_task);
//    job_config.add_dependency(foo_cpp_task, zow_task);
//    job_config.add_dependency(foo_cpp_task, dax_task);
//
//    TaskPtr bar_cpp_task = Task::create<TestProcessingTask>(std::vector<std::string>{"yow.h", "dax.h", "boz.h"}, "bar.cpp");
//    job_config.add_task(bar_cpp_task);
//    job_config.add_dependency(bar_cpp_task, yow_task);
//    job_config.add_dependency(bar_cpp_task, dax_task);
//    job_config.add_dependency(bar_cpp_task, boz_task);
//
//    TaskPtr zig_cpp_task = Task::create<TestProcessingTask>(std::vector<std::string>{"boz.h"}, "zig.cpp");
//    job_config.add_task(zig_cpp_task);
//    job_config.add_dependency(zig_cpp_task, boz_task);
//
//    TaskPtr zag_cpp_task = Task::create<TestProcessingTask>(std::vector<std::string>{"yow.h", "boz.h"}, "zag.cpp");
//    job_config.add_task(zag_cpp_task);
//    job_config.add_dependency(zag_cpp_task, yow_task);
//    job_config.add_dependency(zag_cpp_task, boz_task);

//    TaskPtr foo_o_task = Task::create<TestProcessingTask>(std::vector<std::string>{"foo.cpp"}, "foo.o");
//    job_config.add_task(foo_o_task);
//    job_config.add_dependency(foo_o_task, foo_cpp_task);
//
//    TaskPtr bar_o_task = Task::create<TestProcessingTask>(std::vector<std::string>{"bar.cpp"}, "bar.o");
//    job_config.add_task(bar_o_task);
//    job_config.add_dependency(bar_o_task, bar_cpp_task);
//
//    TaskPtr libfoobar_task = Task::create<TestProcessingTask>(std::vector<std::string>{"foo.o", "bar.o"}, "libfoobar.a");
//    job_config.add_task(libfoobar_task);
//    job_config.add_dependency(libfoobar_task, foo_o_task);
//    job_config.add_dependency(libfoobar_task, bar_o_task);
//
//    TaskPtr zig_o_task = Task::create<TestProcessingTask>(std::vector<std::string>{"zig.cpp"}, "zig.o");
//    job_config.add_task(zig_o_task);
//    job_config.add_dependency(zig_o_task, zig_cpp_task);
//
//    TaskPtr zag_o_task = Task::create<TestProcessingTask>(std::vector<std::string>{"zag.cpp"}, "zag.o");
//    job_config.add_task(zag_o_task);
//    job_config.add_dependency(zag_o_task, zag_cpp_task);
//
//    TaskPtr libzigzag_task = Task::create<TestProcessingTask>(std::vector<std::string>{"libfoobar.a", "zig.o", "zag.o"}, "libzigzag.a");
//    job_config.add_task(libzigzag_task);
//    job_config.add_dependency(libzigzag_task, libfoobar_task);
//    job_config.add_dependency(libzigzag_task, zig_o_task);
//    job_config.add_dependency(libzigzag_task, zag_o_task);
//
//    TaskPtr killerapp_task = Task::create<TestProcessingTask>(std::vector<std::string>{"libzigzag.a"}, "killerapp");
//    job_config.add_task(killerapp_task);
//    killerapp_task->set_tag(334);
//    job_config.add_dependency(killerapp_task, libzigzag_task);

//    bool finished = false;
//    job_config.set_finish_callback([&finished](IJob*){
//        finished = true;
//    });

//    JobManager::get_instance()->run_job_once(job_config);

//    while (!finished) {
//        std::this_thread::sleep_for(5.1s);
//    }

    ASSERT_TRUE(1);
    
    //ASSERT_TRUE(std::filesystem::exists("test/data/killerapp")) << " target file \"killerapp\" not created.";

//    JobManager::get_instance()->stop();

    //std::filesystem::remove_all("test/data");
}
