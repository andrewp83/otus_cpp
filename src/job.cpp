#include "job.h"

#include <boost/graph/topological_sort.hpp>
#include <iostream>

#include "exception.hpp"
#include "thread_pool.h"

namespace task_mgr {

#define PRINT(msg) \
    std::cout << (msg) << std::endl;

// JobConfigurator

void Job::Configurator::add_task(TaskPtr task) {
    tasks.push_back(task);
}

void Job::Configurator::add_dependency(TaskPtr target, TaskPtr source) {
    dependencies.emplace_back(target, source);
}

void Job::Configurator::set_finish_callback(const std::function<void(IJob*)>& callback) {
    finish_callback = callback;
}


// Job

JobPtr Job::create(const Configurator &config, ThreadPool *thread_pool) {
    return std::shared_ptr<Job>(new Job(config, thread_pool));
}

Job::Job(const Job::Configurator& config, ThreadPool* thread_pool) {
    std::for_each(config.tasks.begin(), config.tasks.end(), [this](TaskPtr task){
        add_task(task);
    });
    
    this->thread_pool = thread_pool;
    
    finish_callback = config.finish_callback;
    
    std::for_each(config.dependencies.begin(), config.dependencies.end(), [this](const std::pair<TaskPtr, TaskPtr>& _p){
        add_dependency(_p.first, _p.second);
    });
}

void Job::add_task(TaskPtr task) {
    
    if (task_vertexes.left.count(task) > 0) {
        throw duplicate_task();
    }

    task->set_job(this);
    
    TaskVertex vertex = boost::add_vertex(task, tasks_graph);
    
    task_vertexes.insert({task, vertex});
}

void Job::add_dependency(TaskPtr target, TaskPtr source) {
    
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

TaskPtr Job::get_task_by_vertex(const TaskVertex& vertex) const {
    auto it_task = task_vertexes.right.find(vertex);
    assert(it_task != task_vertexes.right.end() && "task_vertex not found");
    TaskPtr task = it_task->second;
    return task;
}

const Job::TaskVertex& Job::get_vertex_by_task(TaskPtr task) const {
    auto it_vertex = task_vertexes.left.find(task);
    assert(it_vertex != task_vertexes.left.end() && "task not found!");
    const TaskVertex& vertex = it_vertex->second;
    return vertex;
}

void Job::run() {
    if (_self) {
        throw job_is_running();
    }
    
    std::lock_guard<std::mutex> lock(job_mutex);
    
    tasks_completed = 0;
    
    std::for_each(task_vertexes.begin(), task_vertexes.end(), [](boost::bimap<TaskPtr, TaskVertex>::value_type& _p) {
        return _p.get_left_pair().first->init();
    });
    
    _self = shared_from_this();
    
    // SCHEDULE
    make_tasks_order();
    
    // ДЛЯ НАЧАЛА ЗАПУСТИТЬ ВСЕ ТАСКИ, НЕ ЗАВИСЯЩИЕ ОТ ДРУГИХ, Т.Е. БЕЗ ИСХОДЯЩИХ РЕБЕР
   // auto it_begin = tasks_order.begin();
    auto it = std::find_if_not(tasks_order.begin(), tasks_order.end(), [this](const TaskVertex& vertex){
        TaskGraph::out_edge_iterator j, j_end;
        boost::tie(j, j_end) = boost::out_edges(vertex, tasks_graph);
        return j == j_end;
    });
    
    //std::cout << (tasks_order.begin() == it) << std::endl;
    std::for_each(tasks_order.begin(), it, [this](const TaskVertex& vertex) {
        TaskPtr task = get_task_by_vertex(vertex);
        thread_pool->push_task(task);
    });
}

void Job::run_next(TaskPtr task_completed) {
    if (tasks_completed == tasks_order.size()) {
        return;
    }
    
    auto check_and_run_task = [this](TaskPtr task, const TaskVertex& vertex) {
        TaskGraph::out_edge_iterator j, j_end;
        boost::tie(j, j_end) = boost::out_edges(vertex, tasks_graph);
        bool all_finished = std::all_of(j, j_end, [this](const TaskEdge& edge) {
            const TaskVertex& vertex = boost::target(edge, tasks_graph);
            TaskPtr task = get_task_by_vertex(vertex);
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
        TaskPtr task = get_task_by_vertex(vertex);
        check_and_run_task(task, vertex);
    }
}

void Job::task_finished(TaskPtr task) {
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
    auto it = std::find_if(task_vertexes.begin(), task_vertexes.end(), [&](const boost::bimap<TaskPtr, TaskVertex>::value_type& _p) {
        return _p.get_left_pair().first->get_tag() == tag;
    });
    if (it == task_vertexes.end()) {
        return nullptr;
    }
    return it->get_left_pair().first;
}

}
