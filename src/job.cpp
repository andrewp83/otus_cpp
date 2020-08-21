#include "job.h"

//#include "mapper.hpp"
//#include "reducer.hpp"
//
//namespace mr {
//
//
//Job::Job(IMapper* mapper, IReducer* reducer):
//mapper(mapper),
//reducer(reducer) {
//
//}
//
//void Job::set_map_workers_count(std::size_t count) {
//	if (count > 0) {
//		map_workers_count = count;
//	}
//}
//
//void Job::set_reduce_workers_count(std::size_t count) {
//	if (count > 0) {
//		reduce_workers_count = count;
//	}
//}
//
//void Job::wait_for_completion() {
//	std::vector<std::thread> map_workers;
//	map_workers.reserve(map_workers_count);
//	for (std::size_t i = 0; i < map_workers_count; ++i) {
//		map_workers.emplace_back(std::bind(mapper, i));
//	}
//}
//
//}
