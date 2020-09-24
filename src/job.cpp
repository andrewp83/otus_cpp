#include "job.h"

#include <boost/graph/topological_sort.hpp>
#include <iostream>

#include "exception.hpp"
#include "thread_pool.h"

namespace task_mgr {

#define PRINT(msg) \
    std::cout << (msg) << std::endl;

// JobConfigurator

void JobConfigurator::add_task(TaskPtr task) {
    tasks.push_back(task);
}

void JobConfigurator::add_dependency(TaskPtr target, TaskPtr source) {
    dependencies.emplace_back(target, source);
}

void JobConfigurator::set_finish_callback(const std::function<void(IJob*)>& callback) {
    finish_callback = callback;
}

void JobConfigurator::set_tag(int tag) {
    this->tag = tag;
}

// JobTask

JobTask::JobTask(TaskPtr task) {
    this->task = task;
}

void JobTask::run_task() {
    task->run(job);
}

void JobTask::finish() {
    _is_finished = true;
    task->callback(job);
}


// Job

JobPtr Job::create(const JobConfigurator& config, ThreadPool *thread_pool) {
    return std::shared_ptr<Job>(new Job(config, thread_pool));
}

Job::Job(const JobConfigurator& config, ThreadPool* thread_pool) {
    std::unordered_map<TaskPtr, JobTaskPtr> tasks_to_job_tasks;
    
    for (const auto& task : config.get_tasks()) {
        if (tasks_to_job_tasks.count(task) > 0) {
            throw duplicate_task();
        }
        JobTaskPtr job_task = std::make_shared<JobTask>(task);
        add_task(job_task);
        tasks_to_job_tasks[task] = job_task;
    }
    
    this->thread_pool = thread_pool;
    
    finish_callback = config.get_finish_callback();
    
    tag = config.get_tag();
    
    for (const auto& _p : config.get_dependencies()) {
        JobTaskPtr first_job_task = tasks_to_job_tasks[_p.first];
        JobTaskPtr second_job_task = tasks_to_job_tasks[_p.second];
        add_dependency(first_job_task, second_job_task);
    }
}

void Job::add_task(JobTaskPtr task) {

    task->set_job(this);
    
    TaskVertex vertex = boost::add_vertex(task, tasks_graph);
    
    task_vertexes.insert({task, vertex});
}

void Job::add_dependency(JobTaskPtr target, JobTaskPtr source) {
    
    if (task_vertexes.left.count(source) == 0) {
        add_task(source);
    }
    if (task_vertexes.left.count(target) == 0) {
        add_task(target);
    }
    
    auto it_s = task_vertexes.left.find(source);
    TaskVertex source_vertex = it_s->second;
    
    auto it_t = task_vertexes.left.find(target);
    TaskVertex target_vertex = it_t->second;
    
    boost::add_edge(target_vertex, source_vertex, tasks_graph);
    
    bool has_cycle = detect_dependency_cycle();
    if (has_cycle) {
        boost::remove_edge(target_vertex, source_vertex, tasks_graph);
        throw dependency_cycle();
    }
}

bool Job::detect_dependency_cycle() const {
    bool has_cycle = false;
    cycle_detector vis(has_cycle);
    boost::depth_first_search(tasks_graph, boost::visitor(vis));
    return has_cycle;
}

void Job::make_tasks_order() {
    tasks_order.clear();
    boost::topological_sort(tasks_graph, std::back_inserter(tasks_order));
}

JobTaskPtr Job::get_task_by_vertex(const TaskVertex& vertex) const {
    auto it_task = task_vertexes.right.find(vertex);
    assert(it_task != task_vertexes.right.end() && "task_vertex not found");
    JobTaskPtr task = it_task->second;
    return task;
}

const Job::TaskVertex& Job::get_vertex_by_task(JobTaskPtr task) const {
    auto it_vertex = task_vertexes.left.find(task);
    assert(it_vertex != task_vertexes.left.end() && "task not found!");
    const TaskVertex& vertex = it_vertex->second;
    return vertex;
}

void Job::run() {
    
    std::lock_guard<std::mutex> lock(job_mutex);
    
    if (_self) {
        throw job_is_running();
    }
    
    tasks_completed = 0;
    
    for(const auto& _p : task_vertexes) {
        _p.get_left_pair().first->init();
    }
    
    _self = shared_from_this();
    
    // SCHEDULE
    make_tasks_order();
    
    // ДЛЯ НАЧАЛА ЗАПУСТИТЬ ВСЕ ТАСКИ, НЕ ЗАВИСЯЩИЕ ОТ ДРУГИХ, Т.Е. БЕЗ ИСХОДЯЩИХ РЕБЕР
    auto it = std::find_if_not(tasks_order.begin(), tasks_order.end(), [this](const TaskVertex& vertex){
        TaskGraph::out_edge_iterator j, j_end;
        boost::tie(j, j_end) = boost::out_edges(vertex, tasks_graph);
        return j == j_end;
    });
    
    std::for_each(tasks_order.begin(), it, [this](const TaskVertex& vertex) {
        JobTaskPtr task = get_task_by_vertex(vertex);
        thread_pool->push_task(task);
    });
}

void Job::run_next(JobTaskPtr task_completed) {
    if (tasks_completed == tasks_order.size()) {
        return;
    }
    
    auto check_and_run_task = [this](JobTaskPtr task, const TaskVertex& vertex) {
        TaskGraph::out_edge_iterator j, j_end;
        boost::tie(j, j_end) = boost::out_edges(vertex, tasks_graph);
        bool all_finished = std::all_of(j, j_end, [this](const TaskEdge& edge) {
            const TaskVertex& vertex = boost::target(edge, tasks_graph);
            JobTaskPtr task = get_task_by_vertex(vertex);
            return task->is_finished();
        });
        if (all_finished) {
            thread_pool->push_task(task);
        }
    };
    
    // ПРОЙТИ ПО ВСЕМ ВХОДЯЩИМ РЕБРАМ (ЗАВИСЯЩИЕ ЗАДАЧИ)
    const TaskVertex& vertex = get_vertex_by_task(task_completed);
    TaskGraph::in_edge_iterator j, j_end;
    boost::tie(j, j_end) = boost::in_edges(vertex, tasks_graph);
    for(; j != j_end; ++j) {
        const TaskVertex& vertex = boost::source(*j, tasks_graph);
        JobTaskPtr task = get_task_by_vertex(vertex);
        check_and_run_task(task, vertex);
    }
}

void Job::task_finished(JobTaskPtr task) {
    std::lock_guard<std::mutex> lock(job_mutex);
    
    task->finish();
    
    ++tasks_completed;
    
    if (tasks_completed < tasks_order.size()) {
        run_next(task);
    } else if (finish_callback) {
        finish_callback(this);
        _self.reset(); // НА ГРАНИ ФОЛА (ИЛИ ЗА НЕЙ...)
    }
}

TaskPtr Job::get_task_by_tag(int tag) const {
    auto it = std::find_if(task_vertexes.begin(), task_vertexes.end(), [&](const boost::bimap<JobTaskPtr, TaskVertex>::value_type& _p) {
        return _p.get_left_pair().first->get_task()->get_tag() == tag;
    });
    if (it == task_vertexes.end()) {
        return nullptr;
    }
    return it->get_left_pair().first->get_task();
}

void Job::set_tag(int tag) {
    this->tag = tag;
}

int Job::get_tag() const {
    return tag;
}

}
