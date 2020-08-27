#pragma once

namespace mr {

template<class T, class Container = std::vector<T>>
struct IReducer {
	virtual void call(const Container& chunk, std::size_t chunk_num) = 0;
};

}
