#pragma once

#include <vector>


namespace mr {

template<class T, class Container = std::vector<T>>
struct IMapper {
	virtual Container call(std::size_t chunk_num, std::size_t chunks_count) = 0;
};

}
