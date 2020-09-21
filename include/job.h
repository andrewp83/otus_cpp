#pragma once

#include <list>
#include <map>

#include <boost/bimap.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "task.hpp"
#include "thread_pool.h"

namespace task_mgr {

class ThreadWorker;

class Job {
public:
    void add_task(TaskPtr task);
    void add_dependency(TaskPtr target, TaskPtr source);
    
    void run();
    
    void set_finish_callback(const std::function<void(Job*)>& callback);
    
    TaskPtr get_task_by_tag(int tag) const;
    
private:
    using TaskGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, TaskPtr>;
    using TaskVertex = boost::graph_traits<TaskGraph>::vertex_descriptor;
    using TaskEdge = boost::graph_traits<TaskGraph>::edge_descriptor;
    using TaskVertexList = std::list<TaskVertex>;
    
private:
    void run_next(TaskPtr completed_task);
    
    void push_task(TaskPtr task);
    
    void task_finished(TaskPtr task);
    
    bool detect_dependency_cycle() const;
    
    void make_tasks_order();
    
    TaskPtr get_task_by_vertex(const TaskVertex& vertex) const;
    const TaskVertex& get_vertex_by_task(TaskPtr task) const;
    
private:
    friend class ThreadWorker;
        
    TaskGraph tasks_graph;
    
    TaskVertexList tasks_order;
    
    boost::bimap<TaskPtr, TaskVertex> task_vertexes;
    
    
    struct cycle_detector : public boost::dfs_visitor<> {
        
        cycle_detector(bool& has_cycle)
        : _has_cycle(has_cycle) { }

        template <class Edge, class Graph>
        void back_edge(Edge, Graph&) {
          _has_cycle = true;
        }
        
    protected:
        bool& _has_cycle;
    };
    
    std::function<void(Job*)> finish_callback;
    
    static ThreadPool thread_pool;
    
    std::mutex job_mutex;
    
    std::size_t tasks_completed {0};
};

}
