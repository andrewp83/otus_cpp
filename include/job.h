#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>

#include "mapper.h"
#include "reducer.h"

namespace mr {

template<class T, class Container = std::vector<T>>
class Job {
public:
	Job(IMapper<T, Container>* mapper, IReducer<T, Container>* reducer) : mapper(mapper), reducer(reducer) {}

    void set_map_workers_count(std::size_t count) {
        if (count > 0) {
            map_workers_count = count;
        }
    }
    
    void set_reduce_workers_count(std::size_t count) {
        if (count > 0) {
            reduce_workers_count = count;
        }
    }

    void wait_for_completion() {
        std::vector<std::thread> map_workers;
        map_workers.reserve(map_workers_count);
        total_results_size = 0;
        for (std::size_t i = 0; i < map_workers_count; ++i) {
            map_workers.emplace_back(std::bind(&Job::invoke_mapper, this, i, map_workers_count));
        }
        std::for_each(map_workers.begin(), map_workers.end(), [](std::thread& th) {
            th.join();
        });
        
        run_reducers();
    }
    
protected:
    void invoke_mapper(std::size_t chunk_num, std::size_t chunks_count) {
        Container result = mapper->call(chunk_num, chunks_count);
        std::lock_guard<std::mutex> lock(map_results_mutex);
        map_results.push_back(result);
        total_results_size += result.size();
    }
    
    void invoke_reducer(Container chunk, std::size_t chunk_num) {
        reducer->call(chunk, chunk_num);
    }
    
    using T_Iterator = typename Container::const_iterator;
    using T_Iterators_Pair = std::pair<T_Iterator, T_Iterator>;
    
    struct QueueComparator {
        bool operator()(const T_Iterators_Pair& lhs, const T_Iterators_Pair& rhs) const {
            return !(*lhs.first < *rhs.first);
        };
    };
    
    static bool is_equal(const T& lhs, const T& rhs) {
        return !(lhs < rhs) && !(rhs < lhs);
    }
    
    void run_reducers() {
        // merge results
        
        std::vector<std::thread> reduce_workers;
        reduce_workers.reserve(reduce_workers_count);
        
        const std::size_t reduce_chunk_size = std::max(total_results_size / reduce_workers_count, 1UL);
        
        Container merged_result;
        
        std::priority_queue<T_Iterators_Pair, std::vector<T_Iterators_Pair>, QueueComparator> q;
        for(const auto& mr : map_results) {
            q.push(std::make_pair(mr.begin(), mr.end()));
        }
        while (!q.empty()) {
            auto it = q.top().first;
            auto it_end = q.top().second;
            if ((merged_result.size() >= reduce_chunk_size)
                && !(*it == *(merged_result.rbegin()))
                && (reduce_workers.size() < (reduce_workers_count - 1)) ) {
                
                // ЗАПУСТИТЬ ПОТОК REDUCER
                reduce_workers.emplace_back(std::bind(&Job::invoke_reducer, this, std::move(merged_result), reduce_workers.size()));
                merged_result.clear();
                
            } else {
                //std::cout << it->get_data() << std::endl;
                merged_result.push_back(*it);
                q.pop();
                if (++it != it_end) {
                    q.push(std::make_pair(it, it_end));
                }
            }
        }
        if (!merged_result.empty()) {
            reduce_workers.emplace_back(std::bind(&Job::invoke_reducer, this, std::move(merged_result), reduce_workers.size()));
            merged_result.clear();
        }
        
        std::for_each(reduce_workers.begin(), reduce_workers.end(), [](std::thread& th) {
            th.join();
        });
    }

protected:
	IMapper<T, Container>* mapper {nullptr};
	IReducer<T, Container>* reducer {nullptr};
    
    std::list<Container> map_results;
    std::mutex map_results_mutex;
    std::size_t total_results_size {0};

	std::size_t map_workers_count {2};
	std::size_t reduce_workers_count {2};
};

}
