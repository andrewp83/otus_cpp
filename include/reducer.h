#pragma once

namespace mr {

template<class T>
struct IReducer {
	virtual void call(const T& chunk) = 0;
};

}