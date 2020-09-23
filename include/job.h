#pragma once

#include <list>
#include <map>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "task.h"

namespace task_mgr {

class ThreadWorker;
class ThreadPool;

struct IJob {
    virtual TaskPtr get_task_by_tag(int tag) const = 0;
};

using JobPtr = std::shared_ptr<Job>;

class Job : public IJob, public std::enable_shared_from_this<Job> {
public:
    class Configurator {
    public:
        void add_task(TaskPtr task);
        void add_dependency(TaskPtr target, TaskPtr source);
        void set_finish_callback(const std::function<void(IJob*)>& callback);
        void set_tag(int tag);
        
    private:
        friend class Job;
        std::vector<TaskPtr> tasks;
        std::vector<std::pair<TaskPtr, TaskPtr>> dependencies;
        std::function<void(IJob*)> finish_callback;
        int tag {0};
    };
    
public:
    static JobPtr create(const Configurator& config, ThreadPool* thread_pool);
    
    virtual ~Job() {
    }
    
    virtual TaskPtr get_task_by_tag(int tag) const override;
    
private:
    using TaskGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, TaskPtr>;
    using TaskVertex = boost::graph_traits<TaskGraph>::vertex_descriptor;
    using TaskEdge = boost::graph_traits<TaskGraph>::edge_descriptor;
    using TaskVertexList = std::list<TaskVertex>;
    
private:
    Job(const Configurator& config, ThreadPool* thread_pool);
    
    void add_task(TaskPtr task);
    void add_dependency(TaskPtr target, TaskPtr source);
    
    void run();
    
    void run_next(TaskPtr completed_task);
    
    void push_task(TaskPtr task);
    
    void task_finished(TaskPtr task);
    
    bool detect_dependency_cycle() const;
    
    void make_tasks_order();
    
    TaskPtr get_task_by_vertex(const TaskVertex& vertex) const;
    const TaskVertex& get_vertex_by_task(TaskPtr task) const;
    
    void set_tag(int tag);
    int get_tag() const;
    
private:
    friend class ThreadWorker;
    friend class JobManager;
    
    ThreadPool* thread_pool {nullptr};
        
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
    
    std::mutex job_mutex;
    
    std::size_t tasks_completed {0};
    
    JobPtr _self;
    
    int tag {0};
};


}
