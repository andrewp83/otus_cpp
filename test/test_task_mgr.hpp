#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "task.h"
#include "job_manager.h"

using namespace task_mgr;

using namespace std::chrono_literals;

class TestSumTask : public Task {
public:
    TestSumTask(int a, int b) : a(a), b(b) {}
    virtual ~TestSumTask() {}
    
    virtual void run() override {
        sum = a + b;
        result = TaskResult(&sum, sizeof(sum));
        std::this_thread::sleep_for(0.5s);
    }
    
    virtual void callback() override {
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

    Job::Configurator config;

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

    virtual void run() override {
        mul = a * b;
        result = TaskResult(&mul, sizeof(mul));
        std::this_thread::sleep_for(0.5s);
    }

    virtual void callback() override {
        std::cout << "TestMulTask finished " << std::endl;
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

    virtual void run() override {
        res = fact(n);
        result = TaskResult(&res, sizeof(res));
        std::this_thread::sleep_for(0.5s);
    }

    virtual void callback() override {
        std::cout << "TestFactorialTask finished " << std::endl;
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
    std::atomic<int> sum = 0, mul = 0, fact = 0;

    JobManager::get_instance()->start();

    Job::Configurator job_config;

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

    Job::Configurator job_config;

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

    virtual void run() {
        TaskPtr sum_task = get_job()->get_task_by_tag(TAG_SUM_TASK);
        int sum = *((int*)(sum_task->get_result().get_data()));
        TaskPtr mul_task = get_job()->get_task_by_tag(TAG_MUL_TASK);
        int mul = *((int*)(mul_task->get_result().get_data()));
        TaskPtr fact_task = get_job()->get_task_by_tag(TAG_FACT_TASK);
        int fact = *((int*)(fact_task->get_result().get_data()));

        res = (sum + mul + fact) / 3.0f;

        result = TaskResult(&res, sizeof(res));

    }

private:
    float res {0.0f};
};

TEST(test_job, base_dependency_tasks) {

    JobManager::get_instance()->start();

    Job::Configurator job_config;

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

// ПРОВЕРИТЬ НА ВЫБРОС ИСКЛЮЧЕНИЯ ПРИ ДОБАВЛЕНИИ ЦИКЛИЧЕСКИХ ЗАВИСИМОСТЕЙ
