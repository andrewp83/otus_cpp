#pragma once

namespace mr {

template<class T, class Result, class Container = std::vector<T>>
struct IReducer {
	virtual Result call(const Container& chunk) = 0;
};

}
