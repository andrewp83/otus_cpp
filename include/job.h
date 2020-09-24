#pragma once

#include <atomic>
#include <list>
#include <map>
#include <iostream>
#include <mutex>

#include <boost/bimap.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "task_mgr.h"

namespace task_mgr {

class ThreadWorker;
class ThreadPool;
class Job;
using JobPtr = std::shared_ptr<Job>;


class JobTask {
public:
    JobTask(TaskPtr task);
    
    void run_task();
    
    void set_job(Job* job) { this->job = job; }
    Job* get_job() const { return job; }
    
    TaskPtr get_task() const { return task; }
    
    bool is_finished() const { return _is_finished; }
    
    void init() { _is_finished = false; }
    
    void finish();
    
private:
    TaskPtr task;
    std::atomic<bool> _is_finished {0};
    Job* job {nullptr};
};

using JobTaskPtr = std::shared_ptr<JobTask>;


class Job : public IJob, public std::enable_shared_from_this<Job> {
public:
    static JobPtr create(const JobConfigurator& config, ThreadPool* thread_pool);
    
    virtual ~Job() {
    }
    
    virtual TaskPtr get_task_by_tag(int tag) const override;
    
private:
    using TaskGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, JobTaskPtr>;
    using TaskVertex = boost::graph_traits<TaskGraph>::vertex_descriptor;
    using TaskEdge = boost::graph_traits<TaskGraph>::edge_descriptor;
    using TaskVertexList = std::list<TaskVertex>;
    
private:
    Job(const JobConfigurator& config, ThreadPool* thread_pool);
    
    void add_task(JobTaskPtr task);
    void add_dependency(JobTaskPtr target, JobTaskPtr source);
    
    void run();
    
    void run_next(JobTaskPtr completed_task);
    
    void push_task(JobTaskPtr task);
    
    void task_finished(JobTaskPtr task);
    
    bool detect_dependency_cycle() const;
    
    void make_tasks_order();
    
    JobTaskPtr get_task_by_vertex(const TaskVertex& vertex) const;
    const TaskVertex& get_vertex_by_task(JobTaskPtr task) const;
    
    void set_tag(int tag);
    int get_tag() const;
    
private:
    friend class ThreadWorker;
    friend class JobManager;
    friend class JobTask;
    
    ThreadPool* thread_pool {nullptr};
        
    TaskGraph tasks_graph;
    
    TaskVertexList tasks_order;
    
    boost::bimap<JobTaskPtr, TaskVertex> task_vertexes;
    
    
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
